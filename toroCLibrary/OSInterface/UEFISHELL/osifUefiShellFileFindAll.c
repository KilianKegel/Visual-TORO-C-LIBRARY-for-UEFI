/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileFindAll.c

Abstract:

    OS interface (osif) to find all files in a directory for UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>
#include <cde.h>
#include <stddef.h>
#include <stdbool.h>
#include <io.h>
//
// stdio.h
//
extern __declspec(dllimport) FILE* fopen(const char* filename, const char* mode);
extern __declspec(dllimport) size_t fread(const void* ptr, size_t size, size_t nelem, FILE* stream);
extern __declspec(dllimport) int fseek(FILE* stream, long offset, int mode);
#define SEEK_SET    0
// 
// string.h
// 
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strchr(const char* str, int c);
//#include <stdint.h>
// stdlib.h
// 
extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) void* realloc(void* ptr, size_t size);
extern __declspec(dllimport) size_t wcstombs(char* mbstr, const wchar_t* wcstr, size_t count);
extern __declspec(dllimport) size_t mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count);
extern __declspec(dllimport) void abort(void);
//#include <stdbool.h>
// time.h
// 
struct tm
{
    int tm_sec;      /* seconds after the minute � [0, 60]  */
    int tm_min;      /* minutes after the hour � [0, 59]    */
    int tm_hour;     /* hours since midnight � [0, 23]      */
    int tm_mday;     /* day of the month � [1, 31]          */
    int tm_mon;      /* months since January � [0, 11]      */
    int tm_year;     /* years since 1900                    */
    int tm_wday;     /* days since Sunday � [0, 6]          */
    int tm_yday;     /* days since January 1 � [0, 365]     */
    int tm_isdst;    /* Daylight Saving Time flag           */
};
extern __declspec(dllimport) time_t mktime(struct tm* ptm);
//
// string.h
//
extern __declspec(dllimport) char* strncpy(char* pszDst, const char* pszSrc, size_t n);
extern __declspec(dllimport) int strncmp(const char* pszDst, const char* pszSrc, size_t count);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) char* strcat(char* s1, const char* s2);
//
// wchar.h
//
extern __declspec(dllimport) int _wcsicmp(const wchar_t* pszDst, const wchar_t* pszSrc);
extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);

#include <uefi.h>
#include <Protocol\Shell.h>
#define UINT64 unsigned long long
#define UINT8 unsigned char
//#define CDE_APP_IF void

#define ELC(x) (sizeof(x)/sizeof(x[0]))  // element count
#define MAX_FILE_NAME_LEN 522 // (20 * (6+5+2))+1) unicode characters from EFI FAT spec (doubled for bytes)
#define FIND_XXXXX_FILE_BUFFER_SIZE (SIZE_OF_EFI_FILE_INFO + MAX_FILE_NAME_LEN)

extern const short* _CdeGetCurDir(IN CDE_APP_IF *pCdeAppIf, IN const wchar_t* FileSystemMapping);

static CDEFILEINFO* __cdeReadDirectory(IN char* strFileName, OUT int* pcntDirEntries);

/**
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellFileFindAll(CDE_APP_IF* pCdeAppIf, const char* filename, CDEFILE* pCdeFile);
Description
    Find all files in a directory
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    IN char* pstrDrvPthDirStar  : path of search directory followed appended with "\*"
    IN OUT int* pCountOrError   : OUT number
Returns
    0   : success
    EOF : failure
**/
CDEFILEINFO* _osifUefiShellFileFindAll(IN CDE_APP_IF* pCdeAppIf, IN char* pstrDrvPthDirStar, IN OUT int* pCountOrError)
{
    CDEFILEINFO* pCdeFileInfo = NULL;
    bool fIsFileAbs = false/* absolute path vs. relative path */;
    bool fIsFileDrv = false/* a drive name leads the path e.g. FS0: This is identified by precense of ':' */;
    char strDrive[6], strDrive2[6];
    wchar_t wcsDrive2[6];
    unsigned i;

    //
    // non-malloc()'ed pointers
    //
    char* pColon, * pstrCurDir, * pstrCurDir2, * pstrTargetDir, * pDrive, * pCurPath, * pFilePath;

    //
    // malloc()'ed pointers to be freed at _findfirst return , included in freelist[] are initialized with NULL
    //
    wchar_t* pwcsCurDir = NULL, * pwcsCurDir2 = NULL;
    char* pstrFilePath = malloc(320);                                           // can hold 258 bytes max. length of filename
    void* freelist[] = { pwcsCurDir, pstrFilePath };

    if (NULL != pCountOrError)
        *pCountOrError = 0;

    do
    {
        //
        // cut trailing "*" from pstrDrvPthDirStar
        //
        //if (0 == strcmp(pstrDrvPthDirStar,"*"))
        //    strcpy(pstrDrvPthDirStar, ".");
        //else
        pstrDrvPthDirStar[strlen(pstrDrvPthDirStar) - 1] = '\0';
        
        //
        //
        //
        fIsFileDrv = (NULL != (pColon = strchr(pstrDrvPthDirStar, ':')));       // drive name presence is identified by ':'
        fIsFileAbs = pstrDrvPthDirStar[0] == '\\' || (fIsFileDrv ? pColon[1] == '\\' : 0);

        pwcsCurDir = (wchar_t*)_CdeGetCurDir(pCdeAppIf,NULL);
        pstrCurDir = (void*)pwcsCurDir;
        wcstombs(pstrCurDir, pwcsCurDir,(size_t)-1);

        if (fIsFileDrv)                                                         // get current directory of remote drive
        {
            strncpy(strDrive2, pstrDrvPthDirStar, &pColon[1] - pstrDrvPthDirStar);

            mbstowcs(wcsDrive2, strDrive2,(size_t)-1);

            //
            // check if drive name is valid
            //
            if (1)
            {
                for (i = 0; i < pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount; i++)
                {
                    if (0 == _wcsicmp(wcsDrive2, pCdeAppIf->pCdeServices->pCdeSystemVolumes->rgFsVolume[i].rgpVolumeMap[0]))
                        break;
                }

                //CDEMOFINE((MFNINF(i ==gCdeSystemVolumes.nVolumeCount) "Drive %ls %s found\n", wcsDrive2, i ==gCdeSystemVolumes.nVolumeCount ? "NOT" : ""));

                if (i == pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount)
                    break;  // break with nRet == -1, drive is not available

            }

            pwcsCurDir2 = (wchar_t*)_CdeGetCurDir(pCdeAppIf, wcsDrive2);
            pstrCurDir2 = (void*)pwcsCurDir2;
            wcstombs(pstrCurDir2, pwcsCurDir2,(size_t)-1);
        }

        pstrTargetDir = fIsFileDrv ? (0 == strncmp(pstrDrvPthDirStar, pstrCurDir, &pColon[1] - pstrDrvPthDirStar) ? pstrCurDir : pstrCurDir2) : pstrCurDir;

        pDrive = fIsFileDrv ? pstrDrvPthDirStar : pstrTargetDir;                         // getting real drive name of the file
        pColon = strchr(pDrive, (wchar_t)':');
        strncpy(strDrive, pDrive, &pColon[1] - pDrive);
        strDrive[&pColon[1] - pDrive] = '\0';

        pCurPath = 1 + strchr(pstrTargetDir, ':');

        pFilePath = (fIsFileDrv ? 1 + strchr(pstrDrvPthDirStar, ':') : pstrDrvPthDirStar);

        if (fIsFileAbs) {                       // if absolute path ...
            strcpy(pstrFilePath, pFilePath);    // ... just copy the entire path\file
        }
        else {                                  // if relative path...
            strcpy(pstrFilePath, pCurPath);     // ... merge...
            strcat(pstrFilePath, "\\");         // ... the absolute ...
            strcat(pstrFilePath, pFilePath);    // ... path
        }
        //
        // create complete drive + path string
        //
        strcpy(pstrDrvPthDirStar, strDrive);
        strcat(pstrDrvPthDirStar, pstrFilePath);

        //
        // read the UEFI directory structure and convert to CDEFILEINFO
        //
        pCdeFileInfo = __cdeReadDirectory(pstrDrvPthDirStar, pCountOrError);

    } while (0);

    for (i = 0; i < (int)ELC(freelist); i++)
        free(freelist[i]);

    return pCdeFileInfo;
}

static CDEFILEINFO* __cdeReadDirectory(IN char* strFileName, OUT int* pcntDirEntries)
{
    FILE* fp = fopen(strFileName, "r+b");
    size_t sizeFileInfo = 1024;
    EFI_FILE_INFO* FileInfo = malloc(sizeFileInfo);
    size_t sizeCdeFileInfo = sizeof(CDEFILEINFO);               // this is the initial empty structure
    CDEFILEINFO* pCdeFileInfo = malloc(sizeCdeFileInfo);
    CDEFILEINFO* pCdeFileInfoEnd = NULL;
    CDEFILEINFO* pCdeFileInfoFF = NULL;
    int cntDirEntries = 0;                                      // count directory entries
    size_t sizeFileName;

    pCdeFileInfo->time_write = -1LL;

    if (NULL == pCdeFileInfo)
        abort();

    if (NULL != fp) do
    {
        do {
            while (
                sizeFileInfo = 1024,
                sizeFileInfo = fread(FileInfo, 1, sizeFileInfo, fp),
                0 != sizeFileInfo
                )
            {
                //
                // fill "struct tm"-compatible fields
                //
                struct tm timetm = {
                    .tm_sec = FileInfo->ModificationTime.Second,
                    .tm_min = FileInfo->ModificationTime.Minute,
                    .tm_hour = FileInfo->ModificationTime.Hour,
                    .tm_mday = FileInfo->ModificationTime.Day,
                    .tm_mon = FileInfo->ModificationTime.Month,
                    .tm_year = FileInfo->ModificationTime.Year - 1900,
                    .tm_wday = 0,
                    .tm_yday = 0,
                    .tm_isdst = 0
                };
            
                //
                // verify, that data read are truely the "directory" content and not a "file" content
                //
                if (FIND_XXXXX_FILE_BUFFER_SIZE < sizeFileInfo
                    || FileInfo->PhysicalSize % 512
                    || FileInfo->Attribute & ~EFI_FILE_VALID_ATTR
                    )
                {
                    //printf("ERROR: not a directory\n");
                    break;
                }
                fseek(fp, (long)sizeFileInfo, SEEK_SET);

                cntDirEntries++;

                sizeFileName = 1 + wcslen(FileInfo->FileName);
                sizeCdeFileInfo += sizeFileName;
                sizeCdeFileInfo += sizeof(CDEFILEINFO);

                if (1) {
                    void *ptmp = (void*)realloc(pCdeFileInfo, sizeCdeFileInfo);
                    pCdeFileInfo = ptmp;
                    if (NULL == pCdeFileInfo)
                        abort();
                }
                //
                // get pointer to last entry (one before 0xFF/termination structure)
                //
                pCdeFileInfoEnd = (void*)((char*)&pCdeFileInfo[0]
                    + sizeCdeFileInfo
                    - sizeof(CDEFILEINFO)   /*end 0xFF struct */
                    - sizeof(CDEFILEINFO)   /* size of new struct portion*/
                    - sizeFileName)         /* size of the new file name portion*/;
                
                pCdeFileInfoFF = (void*)((char*)&pCdeFileInfo[0]        // get pointer to termination struct
                    + sizeCdeFileInfo
                    - sizeof(CDEFILEINFO))  /*end 0xFF struct */;

                pCdeFileInfoFF->time_write = -1LL;  // write termination signature 0xFFFFFFFFFFFFFFFF (0xFF)

                //
                // fill/copy CdeFileInfo structure elements from EFI_FILE_INFO struct elements
                //
                pCdeFileInfoEnd->attrib = (uint8_t)FileInfo->Attribute;                     // copy attributes
                pCdeFileInfoEnd->size = (_fsize_t)FileInfo->FileSize;                       // copy file size
                wcstombs(&pCdeFileInfoEnd->strFileName[0], FileInfo->FileName,(size_t)-1);  // copy/convert filename
                pCdeFileInfoEnd->time_write = mktime(&timetm);                              // convert EFI_TIME to time_t
 
                if (NULL != pcntDirEntries)
                    (*pcntDirEntries)++;

            }
        } while (0);
    } while (0);
    return pCdeFileInfo;
}
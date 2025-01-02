/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileFindAll.c

Abstract:

    OS interface (osif) to find all files in a directory for Windows NT

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <io.h>
#include <windows.h>
//#undef NMOFINE
//#define MOFINE_CONFIG       MOFINE_STDOUT  | MOFINE_NDRIVER | MOFINE_NFILE |/* MOFINE_NLINE | /*MOFINE_NFUNCTION | */MOFINE_NCLOCK /*| MOFINE_NCLASS | MOFINE_RAWFORMAT */
#include <CdeServices.h>


#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void

/**
Synopsis
    #include <CdeServices.h>
    CDEFILEINFO* _osifWinNTFileFindAll(IN CDE_APP_IF* pCdeAppIf, IN char* pstrDrvPthDirStar, IN OUT int* pCountOrError);
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
CDEFILEINFO* _osifWinNTFileFindAll(IN CDE_APP_IF* pCdeAppIf, IN char* pstrDrvPthDirStar, IN OUT int* pCountOrError)
{
    CDEFILEINFO* pRet = NULL;
    int cntDirEntries = 0;                                      // count directory entries
    size_t sizeFileName;

    WIN32_FIND_DATAA FindData;
    HANDLE hFind;
    size_t sizeCdeFileInfo = sizeof(CDEFILEINFO);               // this is the initial empty structure
    CDEFILEINFO* pCdeFileInfo = malloc(sizeCdeFileInfo);
    CDEFILEINFO* pCdeFileInfoEnd = NULL;
    CDEFILEINFO* pCdeFileInfoFF = NULL;

    if (NULL == pCdeFileInfo)
        abort();

    pCdeFileInfo->time_write = -1LL;

    if (NULL != pCountOrError)
        *pCountOrError = 0;

    do
    {
        bool fFileFound = true;
        
        memset((void*)&FindData, 0, sizeof(FindData));
        hFind = FindFirstFileA(pstrDrvPthDirStar, &FindData);

        while (
            INVALID_HANDLE_VALUE != hFind
            && true == fFileFound
            )
        {

            cntDirEntries++;

            sizeFileName = 1 + strlen(FindData.cFileName);
            sizeCdeFileInfo += sizeFileName;
            sizeCdeFileInfo += sizeof(CDEFILEINFO);

            if (1)
            {
                void* ptmp = realloc(pCdeFileInfo, sizeCdeFileInfo);
                pCdeFileInfo = ptmp;
                if (NULL == pCdeFileInfo)
                    abort();
            }

            //
            // get pointer to last entry (one before 0xFF/termination structure)
            //
            pCdeFileInfoEnd = (void*)((char*)&pCdeFileInfo[0]
                + sizeCdeFileInfo
                - sizeof(CDEFILEINFO)   /* end 0xFF struct */
                - sizeof(CDEFILEINFO)   /* size of new struct portion*/
                - sizeFileName)         /* size of the new file name portion*/;

            pCdeFileInfoFF = (void*)((char*)&pCdeFileInfo[0]        // get pointer to termination struct
                + sizeCdeFileInfo
                - sizeof(CDEFILEINFO))  /*end 0xFF struct */;

            pCdeFileInfoFF->time_write = -1LL;  // write termination signature 0xFFFFFFFFFFFFFFFF (0xFF)

            //
            // fill/copy CdeFileInfo structure elements from EFI_FILE_INFO struct elements
            //
            pCdeFileInfoEnd->size = FindData.nFileSizeLow + (((uint64_t)FindData.nFileSizeHigh) << 32LL);
            pCdeFileInfoEnd->attrib = (uint8_t)FindData.dwFileAttributes;       // copy attributes
            strcpy(&pCdeFileInfoEnd->strFileName[0], FindData.cFileName);       // copy/convert filename
            
            //
            // convert windows file time to system time an back to C time
            //
            if (1)
            {
                SYSTEMTIME  SystemTime;
                struct tm cTime = {0,0,0,0,0,0,0,0,0};
                BOOL fRet = 0;

                fRet = FileTimeToSystemTime(
                    &FindData.ftLastWriteTime,
                    &SystemTime);

                if (0 != fRet)
                {
                    cTime.tm_year = SystemTime.wYear - 1900;
                    cTime.tm_mon = SystemTime.wMonth - 1;
                    cTime.tm_mday = SystemTime.wDay;
                    cTime.tm_hour = SystemTime.wHour;
                    cTime.tm_min = SystemTime.wMinute;
                    cTime.tm_sec = SystemTime.wSecond;

                    pCdeFileInfoEnd->time_write = mktime(&cTime);
                }else
                    pCdeFileInfoEnd->time_write = 0;
            }
            if (NULL != pCountOrError)
                (*pCountOrError)++;


            memset((void*)&FindData, 0, sizeof(FindData));
            fFileFound =  FindNextFileA(hFind, &FindData);
        }

        pRet = pCdeFileInfo;

    } while (0);

    return pRet;
}

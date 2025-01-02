/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileGetStatus.c

Abstract:

    OS interface (osif) get status of a file, support of POSIX "stat()" function

Author:

    Kilian Kegel

--*/
#define OS_EFI//parameter for CdeServices.h
#include <CdeServices.h>
#include <stdio.h>

//
// stdlib.h
//
extern __declspec(dllimport) long strtol(const char* s, char** endptr, int base);
extern __declspec(dllimport) size_t wcstombs(char* mbstr, const wchar_t* wcstr, size_t count);
//
// string.h
//
extern __declspec(dllimport) char* strstr(const char* pszStr, const char* pszSubStr);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) int _strnicmp(const char* pszDst, const char* pszSrc, size_t count);
//
// ctype.h
//
extern __declspec(dllimport) int toupper(int c);

#include <sys/stat.h>
#include <guid/fileinfo.h>

extern EFI_GUID _gEfiFileInfoIdGuid;
extern time_t _cdeEfiTime2TimeT(EFI_TIME* pEfiTime);

/**
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellFileGetStatus(IN CDE_APP_IF* pCdeAppIf, void* pFpOrFname, CDESTAT64I32 *pStat64i32, void* pWorkBuf400);
Description
    Get file status
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    void* pFpOrFname            : filepointer or filename
    CDESTAT64I32 *pStat64i32    : pointer to struct tagCDESTAT64I32
    void* pWorkBuf400           : pointer to temporary buffer of 400 bytes
Returns
     0  : success
    -1  : failure
**/
int _osifUefiShellFileGetStatus(IN CDE_APP_IF* pCdeAppIf, void* pFpOrFname, CDESTAT64I32 *pStat64i32, void* pWorkBuf400)
{
    CDEFILE* pCdeFile = pFpOrFname;
    EFI_STATUS Status;
    size_t FileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(wchar_t) * 255/* max. FAT filename length */;
    EFI_FILE_INFO* pFileInfo = pWorkBuf400;
    int nRet = -1;

    do {

        Status = pCdeFile->pFileProtocol->GetInfo(pCdeFile->pFileProtocol, &_gEfiFileInfoIdGuid, &FileInfoSize, pFileInfo);

        if (EFI_SUCCESS != Status)
            break;

        //
        // st_dev, st_rdev: convert FSx: to integer drive number, 0 based
        //
        if (1) {
            char drivename[16];
            char* pColon;

            wcstombs(drivename, pCdeFile->pwcsFileDrive, sizeof(drivename));  // convert wcs to str since
                // regrettably a "wcstol()" function is _NOT_ yet implemented
            pColon = strstr(drivename, ":");

            // distiguish between FSx: and A:..Z: mapping
            if (0 == _strnicmp(drivename, "FS", sizeof("FS")))
                pStat64i32->st_dev = (int)strtol(&drivename[strlen("FS")], &pColon, 10);
            else
                pStat64i32->st_dev = toupper(drivename[0]) - 'A';
            pStat64i32->st_rdev = pStat64i32->st_dev;
        }
        pStat64i32->st_nlink = 1;
        pStat64i32->st_ino = 0;
        pStat64i32->st_uid = 0;
        pStat64i32->st_gid = 0;

        pStat64i32->st_size = (_off_t)pFileInfo->FileSize;
        pStat64i32->st_ctime = _cdeEfiTime2TimeT(&pFileInfo->CreateTime);
        pStat64i32->st_mtime = _cdeEfiTime2TimeT(&pFileInfo->ModificationTime);
        //pStat64i32->st_atime = _cdeEfiTime2TimeT(&pFileInfo->LastAccessTime); // BUGFIX: atime's timestamp is 0:0:0
        pStat64i32->st_atime = pStat64i32->st_mtime;

        pStat64i32->st_mode = 0;
        pStat64i32->st_mode |= ((_S_IFREG + _S_IREAD + 0x24/*undocumented flags*/) * (0 == (EFI_FILE_DIRECTORY & pFileInfo->Attribute)));
        pStat64i32->st_mode |= ((_S_IFDIR + _S_IREAD + _S_IEXEC + 0x2D/*undocumented flags*/) * (0 != (EFI_FILE_DIRECTORY & pFileInfo->Attribute)));
        pStat64i32->st_mode |= ((_S_IWRITE + 0x32/*undocumented flags*/) * (0 == (EFI_FILE_READ_ONLY & pFileInfo->Attribute)));

    } while (0 != (nRet = 0));
    
    return nRet;
}
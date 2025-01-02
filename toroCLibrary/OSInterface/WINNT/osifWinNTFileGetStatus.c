/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileGetStatus.c

Abstract:

    OS interface (osif) get status of a file, support of POSIX "stat()" function

Author:

    Kilian Kegel

--*/
#include <io.h>
#include <sys/stat.h>
#include <windows.h>

#include <cde.h>
#include <CdeServices.h>

#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void

/**
Synopsis
    #include <CdeServices.h>
    int _osifWinNTFileGetStatus(IN CDE_APP_IF* pCdeAppIf, void* pFpOrFname, CDESTAT64I32 *pStat64i32, void* pWorkBuf400);
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
int _osifWinNTFileGetStatus(IN CDE_APP_IF* pCdeAppIf, void* pFpOrFname, CDESTAT64I32* pStat64i32, void* pBuf400)
{
    char* pstrFname = pFpOrFname;
    WIN32_FILE_ATTRIBUTE_DATA FileInfo;
    int nRet = -1; 
    char* pBuf = pBuf400;

    do
    {
        if (0 == GetFileAttributesExA(pstrFname, GetFileExInfoStandard, &FileInfo))
            break;

        //
        // st_dev, st_rdev: convert drive A:..Z: to integer drive number, 0 based
        //
        if (1) 
        {
            pBuf[0] = 0;
            pBuf[1] = 0;
            GetFullPathNameA(pFpOrFname, 0x400, pBuf, NULL);
            
            if (':' == pBuf[1])
                pStat64i32->st_dev = toupper(pBuf[0]) - 'A' ;
            else
                pStat64i32->st_dev = 0;// in case of doubt: A:
            pStat64i32->st_rdev = pStat64i32->st_dev;
        }

        pStat64i32->st_nlink = 1;
        pStat64i32->st_ino = 0;
        pStat64i32->st_uid = 0;
        pStat64i32->st_gid = 0;

        pStat64i32->st_size = FileInfo.nFileSizeLow;
        //
        // NOTE: Windows FILETIME is 100-nanosecond intervals since January 1, 1601 (UTC). 
        //       Unix time is seconds since January 1, 1970 (UTC).
        //       // https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
        //
        pStat64i32->st_atime =
            -11644473600LL +    // 1970-1601 in seconds
            ((time_t)FileInfo.ftLastAccessTime.dwLowDateTime +
                (time_t)FileInfo.ftLastAccessTime.dwHighDateTime * 0x100000000LL) / 10000000LL;
        pStat64i32->st_ctime =
            -11644473600LL +    // 1970-1601 in seconds
            ((time_t)FileInfo.ftCreationTime.dwLowDateTime +
                (time_t)FileInfo.ftCreationTime.dwHighDateTime * 0x100000000LL) / 10000000LL;
        pStat64i32->st_mtime =
            -11644473600LL +    // 1970-1601 in seconds
            ((time_t)FileInfo.ftLastWriteTime.dwLowDateTime +
                (time_t)FileInfo.ftLastWriteTime.dwHighDateTime * 0x100000000LL) / 10000000LL;

        pStat64i32->st_mode = 0;
        pStat64i32->st_mode |= ((_S_IFREG + _S_IREAD + 0x24/*undocumented flags*/) * (0 == (_A_SUBDIR & FileInfo.dwFileAttributes)));
        pStat64i32->st_mode |= ((_S_IFDIR + _S_IREAD + _S_IEXEC + 0x2D/*undocumented flags*/) * (0 != (_A_SUBDIR & FileInfo.dwFileAttributes)));
        pStat64i32->st_mode |= ((_S_IWRITE + 0x32/*undocumented flags*/) * (0 == (_A_RDONLY & FileInfo.dwFileAttributes)));

    } while (0 != (nRet = 0));

    return nRet;
}

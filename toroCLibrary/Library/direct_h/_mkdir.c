/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _mkdir.c

Abstract:

    Implementation of the Microsoft C function

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <CdeServices.h>


/** _mkdir
Synopsis
    #include <direct.h>
    int _mkdir(const char *dirname);
Description
    Creates a new directory.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkdir-wmkdir?view=msvc-160#parameters
Returns
    NOTE: Return value description is wrong at Microsoft link
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkdir-wmkdir?view=msvc-160#return-value
    ERRNO EEXIST 17: File exists                exist
    ERRNO EACCES 13: Permission denied          write protect
    ERRNO ENOENT 2 : No such file or directory  drive not present
    nErrRet on succs 0
    nErrRet on error -1
**/

int _mkdir(const char* pstrDirName)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int nErrRet = -1 /* assume error */;
    struct _stat64i32 stat;
    int n;
    int errnoold = errno;

    errno = 0;

    if (NULL != pCdeAppIf) {
        size_t lenStrDirName = strnlen(pstrDirName, CDE_FILESYSNAME_LEN_MAX);
        size_t sizeWcsDirName = sizeof(L"") + sizeof(wchar_t) * lenStrDirName;
        wchar_t* pwcsDirName = malloc(CDE_FILESYSNAME_LEN_MAX);
        
        do {

            if (CDE_FILESYSNAME_LEN_MAX == lenStrDirName)
                break;
            //
            // check if directory/file name already exists
            //
            n = _stat64i32(pstrDirName, &stat);

            if ((/* 2, no such file or directory */ ENOENT == errno && -1 == n))
            {
                mbstowcs(pwcsDirName, pstrDirName, sizeWcsDirName);  // convert string to wide character string

                n = pCdeAppIf->pCdeServices->pDirCreate(pCdeAppIf, pwcsDirName);

                if (0 != n) {
                    errno = n;  // CreateDirectory() failed - e.g. drive fsX: n/a
                    break;      // break with error in nErrRet
                }   
                else
                    errno = 0;  // clear errno due to success of operation

            }
            else {
                // file / directory name already exists
                errno = EEXIST;
                break;      // break with error in nErrRet
            }

        } while (nErrRet = EXIT_SUCCESS, nErrRet == EXIT_SUCCESS);   // exit loop with success of operation

        if (NULL != pwcsDirName)
            free(pwcsDirName);
    }
    
    //
    // in case or NO ERRNO, replace previous ERRNO
    //
    if (errno == 0)
        errno = errnoold;

    return nErrRet;

}
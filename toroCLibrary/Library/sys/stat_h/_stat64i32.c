/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _stat64i32.c

Abstract:

    Implementation of the Microsoft / POSIX C function.
    Get status information on a file.
    Microsoft _stat64i32() equivalent to POSIX stat()

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/** _stat64i32()
Synopsis
    #include <sys/stat.h>
    int _stat64i32(char const* _FileName, struct _stat64i32* _Stat);
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-160#syntax
    https://pubs.opengroup.org/onlinepubs/009696799/functions/stat.html
Description
    Get status information on a file.
Paramters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-160#return-value
**/
int _stat64i32(char const* fname, CDESTAT64I32/*struct _stat64i32*/* fstat) {

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int nRet = -1;                              // presume failure

    if (NULL != pCdeAppIf) 
    {
        void* pBuf400 = malloc(0x400);
    
        do {
            //
            // NOTE: UEFI determines file status based on an OPEN FILE
            //       Windows determines file status based on an FILE NAME
            void* pFpOrFname = (void*) & fname[0];   // presume Windows, take file name string

            if(SHELLIF == pCdeAppIf->DriverParm.CommParm.OSIf)
                pFpOrFname = fopen(fname, "r");

            if (NULL == pFpOrFname)
                break;

            //printf(__FILE__"(%d)\\"__FUNCTION__"(): %s fp %p\n", __LINE__, fname, pFpOrFname);

            nRet = pCdeAppIf->pCdeServices->pFgetstatus(pCdeAppIf, pFpOrFname, (CDESTAT64I32 *)fstat, pBuf400);


            if (SHELLIF == pCdeAppIf->DriverParm.CommParm.OSIf)
                fclose((FILE*)pFpOrFname);

        } while (0);

        free(pBuf400);

        if (-1 == nRet)
            errno = ENOENT;
    }

    return nRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    errnoCORE.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <errno.h>
    int* _errno(void)
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-doserrno-sys-errlist-and-sys-nerr?view=msvc-160&viewFallbackFrom=vs-2019

Description

    Microsoft C Library specific function used in the Standard C "errno" macro

Parameter

Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-doserrno-sys-errlist-and-sys-nerr?view=msvc-160#remarks
**/
int* _errnoCORE(CDE_APP_IF *pCdeAppIf)
{
    int* pRet = NULL;

    if (NULL != pCdeAppIf) 
        pRet = &pCdeAppIf->nErrno;

    return pRet;
}
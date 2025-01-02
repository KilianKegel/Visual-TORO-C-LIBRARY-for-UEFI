/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrCpy.c

Abstract:

    Implementation of the Standard C function.
    Copies a string

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

/**

Synopsis
    #include <string.h>
    char* strcpy(char* pszDst, const char* pszSrc);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcpy-wcscpy-mbscpy?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcpy-wcscpy-mbscpy?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcpy-wcscpy-mbscpy?view=msvc-160#return-value
**/
char* strcpy(char* pszDst, const char* pszSrc) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return (char*)pCdeAppIf->pCdeServices->pMemStrxCpy(BOZ, pszDst, pszSrc, (size_t)-1);

}
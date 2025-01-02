/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrNCpyCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Copy number of characters of one string to another. 

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

/**
Synopsis
    #include <string.h>
    char* strncpy(char* pszDst, const char* pszSrc, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strncpy-strncpy-l-wcsncpy-wcsncpy-l-mbsncpy-mbsncpy-l?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strncpy-strncpy-l-wcsncpy-wcsncpy-l-mbsncpy-mbsncpy-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strncpy-strncpy-l-wcsncpy-wcsncpy-l-mbsncpy-mbsncpy-l?view=msvc-160#return-value
**/
static char* strncpyCDEABI(char* pszDst, const char* pszSrc, size_t n) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return (char*)pCdeAppIf->pCdeServices->pMemStrxCpy(BOZ + CIP, pszDst, pszSrc, n);

}

MKCDEABI(strncpy);
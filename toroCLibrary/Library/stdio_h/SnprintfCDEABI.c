/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    SnprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Writes formatted data to a string.

Author:

    Kilian Kegel

--*/
#include <stdarg.h>
#include <CdeServices.h>

extern int __declspec(dllimport) vsnprintf(char* pszDest, size_t dwCount, const char* pszFormat, va_list ap);

/**

Synopsis
    #include <stdio.h>
    int snprintf(char* pszDest, size_t dwCount, const char* pszFormat, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/snprintf-snprintf-snprintf-l-snwprintf-snwprintf-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/snprintf-snprintf-snprintf-l-snwprintf-snwprintf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/snprintf-snprintf-snprintf-l-snwprintf-snwprintf-l?view=msvc-160#return-value
**/
static int snprintfCDEABI(char* pszDest, size_t dwCount, const char* pszFormat, ...) {
    va_list ap;
    int nRet;

    va_start(ap, pszFormat);

    nRet = vsnprintf(pszDest, dwCount, pszFormat, ap);

    va_end(ap);

    return nRet;
}

MKCDEABI(snprintf);
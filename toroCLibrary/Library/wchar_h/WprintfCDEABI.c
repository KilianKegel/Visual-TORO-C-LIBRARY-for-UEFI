/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    WprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Prints formatted output to the standard output stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void
#define stdout (__acrt_iob_func(1))

extern FILE* __acrt_iob_func(unsigned i);
extern __declspec(dllimport) int vfwprintf(FILE* stream, const wchar_t* pszFormat, va_list ap);

/**
Synopsis
    #include <wchar.h>
    wchar_t* wmemcpy(wchar_t* s1, const wchar_t* s2, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-160
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=378
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-160#return-value
**/
static int wprintfCDEABI(const wchar_t* pszFormat, ...) {

    va_list ap;
    int nRet = 0;

    va_start(ap, pszFormat);

    nRet = vfwprintf(stdout, pszFormat, ap);

    va_end(ap);

    return nRet;
}

MKCDEABI(wprintf);
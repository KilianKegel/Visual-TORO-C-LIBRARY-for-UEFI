/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FwprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Print formatted data to a stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern __declspec(dllimport) int vfwprintf(FILE* stream, const wchar_t* pszFormat, va_list ap);

/**
Synopsis
    #include <stdio.h>
    #include <wchar.h>
    int fwprintf(FILE * restrict stream, const wchar_t * restrict format, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fprintf-fprintf-l-fwprintf-fwprintf-l?view=msvc-170&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=361
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fprintf-fprintf-l-fwprintf-fwprintf-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fprintf-fprintf-l-fwprintf-fwprintf-l?view=msvc-170#return-value
**/
static int fwprintfCDEABI(FILE* stream, const wchar_t* pszFormat, ...) {

    va_list ap;
    int nRet = 0;

    va_start(ap, pszFormat);

    nRet = vfwprintf(stream, pszFormat, ap);

    va_end(ap);

    return nRet;
}

MKCDEABI(fwprintf);
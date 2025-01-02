/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ScanfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Reads formatted data from the standard input stream. 

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern __declspec(dllimport) int vfscanf(FILE* stream, const char* pszFormat, va_list ap);
/**
Synopsis
    #include <stdio.h>
    int scanf(const char* pszFormat, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l?view=msvc-160&viewFallbackFrom=vs-2019
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l?view=msvc-160#return-value

**/
static int scanfCDEABI(const char* pszFormat, ...) {
    va_list ap;
    int nRet;

    va_start(ap, pszFormat);


    nRet = vfscanf((FILE*)CDE_STDIN, pszFormat, ap);

    va_end(ap);

    return nRet;

}

MKCDEABI(scanf);
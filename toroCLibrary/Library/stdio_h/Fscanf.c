/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fscanf.c

Abstract:

    Implementation of the Standard C function.
    Read formatted data from a stream.
    
Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdarg.h>

/**
Synopsis

    #include <stdio.h
    int fscanf(FILE* stream, const char* pszFormat, ...);
Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fscanf-fscanf-l-fwscanf-fwscanf-l?view=msvc-160&viewFallbackFrom=vs-2019

Paramters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fscanf-fscanf-l-fwscanf-fwscanf-l?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fscanf-fscanf-l-fwscanf-fwscanf-l?view=msvc-160#return-value

**/
int fscanf(FILE* const stream, char const* const pszFormat, ...) {
    va_list ap;
    int nRet;

    va_start(ap, pszFormat);

    nRet = vfscanf(stream, pszFormat, ap);

    va_end(ap);

    return nRet;
}

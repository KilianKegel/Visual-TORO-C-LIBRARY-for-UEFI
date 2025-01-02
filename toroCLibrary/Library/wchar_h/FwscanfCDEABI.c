/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    fwscanfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Reads formatted data from a wide string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern __declspec(dllimport) int vfwscanf(FILE* stream, const wchar_t* pwcsFormat, va_list ap);

/**

Synopsis
    #include <stdio.h>
    #include <wchar.h>
    int fwscanf(FILE * restrict stream, const wchar_t * restrict format, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fscanf-fscanf-l-fwscanf-fwscanf-l?view=msvc-170
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=376
    The fwscanf function reads input from the stream pointed to by stream, under
    control of the wide string pointed to by format that specifies the admissible input
    sequences and how they are to be converted for assignment, using subsequent arguments
    as pointers to the objects to receive the converted input. If there are insufficient
    arguments for the format, the behavior is undefined. If the format is exhausted while
    arguments remain, the excess arguments are evaluated (as always) but are otherwise
    ignored.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fscanf-fscanf-l-fwscanf-fwscanf-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fscanf-fscanf-l-fwscanf-fwscanf-l?view=msvc-170#return-value
**/
static int fwscanfCDEABI(FILE* stream, const wchar_t* pwcsFormat, ...) {
    int nRet;
    va_list ap;

    va_start(ap, pwcsFormat);

    nRet = vfwscanf(stream, pwcsFormat, ap);

    va_end(ap);

    return nRet;
}

MKCDEABI(fwscanf);
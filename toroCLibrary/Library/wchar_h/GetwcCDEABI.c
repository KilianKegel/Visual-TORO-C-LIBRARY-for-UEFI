/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    GetwcCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Read a wide character from a stream.


Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void
//
// wchar.h
//
typedef unsigned short wint_t;

extern __declspec(dllimport) wint_t fgetwc(FILE* stream);

/** fgetwc
Synopsis
    #include <wchar.h>
    wint_t getwc(FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetc-fgetwc?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=381
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetc-fgetwc?view=msvc-160#return-value
**/
static wint_t getwcCDEABI(FILE* stream) 
{
    return fgetwc(stream);
}

MKCDEABI(getwc);
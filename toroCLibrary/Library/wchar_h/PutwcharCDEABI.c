/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    PutwcharCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Writes a wide character to stdout.

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

extern __declspec(dllimport) wint_t fputwc(wchar_t c, FILE* stream);

/**
Synopsis
    #include <wchar.h>
    wint_t putwchar(wchar_t c);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/putchar-putwchar?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=382
    The putwchar function is equivalent to putwc with the second argument stdout.
Returns
    The putwchar function returns the character written, or WEOF.

**/
static wint_t putwcharCDEABI(wchar_t c)
{
    return fputwc(c, (FILE*)CDE_STDOUT);
}

MKCDEABI(putwchar);
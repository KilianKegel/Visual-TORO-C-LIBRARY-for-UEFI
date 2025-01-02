/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    GetcharCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Reads a wide character from standard input.

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

#ifdef getwchar
#   undef getwchar
#endif//getchar

/** getwchar
Synopsis
    #include <wchar.h>
    wint_t getwchar(void);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getchar-getwchar?view=msvc-160
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=381
    The getwchar() function is equivalent to getwc with the argument stdin.
Returns
    The getwchar function returns the next wide character from the input stream pointed to
    by stdin, or WEOF.
**/
static wint_t getwcharCDEABI(void)
{
    return fgetwc((FILE*)CDE_STDIN);
}

MKCDEABI(getwchar);
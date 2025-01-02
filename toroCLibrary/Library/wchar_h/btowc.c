/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    btowc.c

Abstract:

    Implementation of the Standard C function.
    Determine whether an integer represents a valid single-byte character in the initial shift state.

Author:

    Kilian Kegel

--*/
#include <stdio.h>

/**
Synopsis
    #include <stdio.h>
    #include <wchar.h>
    wint_t btowc(int c);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/btowc?view=msvc-170&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=399
    The btowc function determines whether c constitutes a valid single-byte character in the
    initial shift state.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/btowc?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/btowc?view=msvc-170#return-value
    The btowc function returns WEOF if c has the value EOF or if (unsigned char)c
    does not constitute a valid single-byte character in the initial shift state. Otherwise, it
    returns the wide character representation of that character.
**/
wint_t btowc(int c)
{
    wint_t nRet = WEOF;

    if (EOF != c)
        nRet = 0xFF & c;

    return nRet;
}

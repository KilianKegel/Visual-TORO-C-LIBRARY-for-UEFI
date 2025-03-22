/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wctobCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Determines if a wide character corresponds to a multibyte character and returns its multibyte character representation.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) int wctomb(char* pmb, wchar_t wc);

/**
Synopsis
    #include <wchar.h>
    wchar_t* wcstok(wchar_t* pszStr, const wchar_t* pszSet, wchar_t** ppLast);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wctob?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wctob?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wctob?view=msvc-160#return-value
**/
static int wctobCDEABI(wchar_t wc)
{
    char c;

    return 1 == wctomb(&c, wc) ? 0xFF & c : -1;
}

MKCDEABI(wctob);
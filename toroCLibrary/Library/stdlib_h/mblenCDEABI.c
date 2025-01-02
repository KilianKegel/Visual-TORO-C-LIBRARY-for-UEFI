/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mblenCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Gets the length and determines the validity of a multibyte character.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

extern __declspec(dllimport) int mbtowc(wchar_t* wchar, const char* mbchar, size_t count);

/**

Synopsis
    #include <stdlib.h>
    int mblen(const char* s, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbclen-mblen-mblen-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbclen-mblen-mblen-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbclen-mblen-mblen-l?view=msvc-160#return-value
**/
static int mblenCDEABI(const char* s, size_t n)
{
    return mbtowc(NULL, s, n);
}

MKCDEABI(mblen);
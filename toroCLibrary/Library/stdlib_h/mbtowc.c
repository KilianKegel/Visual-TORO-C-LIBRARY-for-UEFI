/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbtowc.c

Abstract:

    Implementation of the Standard C function.
    Convert a multibyte character to a corresponding wide character.

Author:

    Kilian Kegel

--*/
#include <stddef.h>

/**

Synopsis
    #include <stdlib.h>
    size_t mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbtowc-mbtowc-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbtowc-mbtowc-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbtowc-mbtowc-l?view=msvc-160#return-value
**/
int mbtowc(wchar_t* wchar, const char* mbchar, size_t count) {

    int nRet = (count > 0 ? (NULL != mbchar ? ('\0' != *mbchar ? 1 : 0) : 0) : 0);

    if (0 != count && NULL != wchar && NULL != mbchar)
        *wchar = 0xFF & (wchar_t)*mbchar;

    return nRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Wcsxfrm.c

Abstract:

    Implementation of the Standard C function.
    Transform a wide string based on locale-specific information.

Author:

    Kilian Kegel

TODO: add invalid parameter handler support

--*/

#include <wchar.h>

/**
Synopsis
    #include <string.h>
    #include <wchar.h>
    size_t wcsxfrm(wchar_t * restrict s1, const wchar_t * restrict s2, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strxfrm-wcsxfrm-strxfrm-l-wcsxfrm-l?view=msvc-170&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=392
    The wcsxfrm function transforms the wide string pointed to by s2 and places the
    resulting wide string into the array pointed to by s1. The transformation is such that if
    the wcscmp function is applied to two transformed wide strings, it returns a value greater
    than, equal to, or less than zero, corresponding to the result of the wcscoll function
    applied to the same two original wide strings. No more than n wide characters are placed
    into the resulting array pointed to by s1, including the terminating null wide character. If
    nis zero, s1 is permitted to be a null pointer.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strxfrm-wcsxfrm-strxfrm-l-wcsxfrm-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strxfrm-wcsxfrm-strxfrm-l-wcsxfrm-l?view=msvc-170#return-value
**/
size_t wcsxfrm(wchar_t* pszDst, const wchar_t* pszSrc, size_t n) {

    return wcslen(wcsncpy(pszDst, pszSrc, n));
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    WcsColl.c

Abstract:

    Implementation of the Standard C function.
    Compares wide strings by using the current locale or a specified LC_COLLATE conversion-state category.

Author:

    Kilian Kegel

--*/
#include <wchar.h>

/**

Synopsis
    #include <wchar.h>
    int wcscoll(const wchar_t *s1, const wchar_t *s2);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcoll-wcscoll-mbscoll-strcoll-l-wcscoll-l-mbscoll-l?view=msvc-170&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=391
    The wcscoll function compares the wide string pointed to by s1 to the wide string
    pointed to by s2, both interpreted as appropriate to the LC_COLLATE category of the
    current locale.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcoll-wcscoll-mbscoll-strcoll-l-wcscoll-l-mbscoll-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcoll-wcscoll-mbscoll-strcoll-l-wcscoll-l-mbscoll-l?view=msvc-170#return-value
    The wcscoll function returns an integer greater than, equal to, or less than zero,
    accordingly as the wide string pointed to by s1 is greater than, equal to, or less than the
    wide string pointed to by s2 when both are interpreted as appropriate to the current
    locale.
**/
int wcscoll(const wchar_t* pszDst, const wchar_t* pszSrc)
{
    return wcscmp(pszDst,pszSrc);
}
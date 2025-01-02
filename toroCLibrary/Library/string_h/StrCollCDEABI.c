/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrCollCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Compares strings by using the current locale or a specified LC_COLLATE conversion-state category.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) int strcmp(const char* pszDst, const char* pszSrc);

/**

Synopsis
    #include <string.h>
    int strcoll(const char *s1, const char *s2);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcoll-wcscoll-mbscoll-strcoll-l-wcscoll-l-mbscoll-l?view=msvc-170&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=341
    The strcoll function compares the string pointed to by s1 to the string pointed to by
    s2, both interpreted as appropriate to the LC_COLLATE category of the current locale.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcoll-wcscoll-mbscoll-strcoll-l-wcscoll-l-mbscoll-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcoll-wcscoll-mbscoll-strcoll-l-wcscoll-l-mbscoll-l?view=msvc-170#return-value
    The strcoll function returns an integer greater than, equal to, or less than zero,
    accordingly as the string pointed to by s1 is greater than, equal to, or less than the string
    pointed to by s2 when both are interpreted as appropriate to the current locale.
**/
static int strcollCDEABI(const char* pszDst, const char* pszSrc) 
{
    return strcmp(pszDst,pszSrc);
}

MKCDEABI(strcoll);
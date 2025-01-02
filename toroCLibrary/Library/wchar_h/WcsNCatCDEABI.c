/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    WcsNCatCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Appends characters of a wide string. 

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);
extern __declspec(dllimport) wchar_t* wcsncpy(wchar_t* pszDst, const wchar_t* pszSrc, size_t n);

/**
Synopsis
    #include <wchar.h>
    wchar_t* wcsncat(wchar_t* s1, const wchar_t* s2, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strncat-strncat-l-wcsncat-wcsncat-l-mbsncat-mbsncat-l?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strncat-strncat-l-wcsncat-wcsncat-l-mbsncat-mbsncat-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strncat-strncat-l-wcsncat-wcsncat-l-mbsncat-mbsncat-l?view=msvc-160#return-value
**/
static wchar_t* wcsncatCDEABI(wchar_t* s1, const wchar_t* s2, size_t n) {
    size_t size1 = wcslen(s1);
    size_t size2 = wcslen(s2);
    size_t i = size2 < n ? size2 : n;

    wcsncpy(&s1[size1], s2, i);
    s1[size1 + i] = 0;

    return s1;
}

MKCDEABI(wcsncat);
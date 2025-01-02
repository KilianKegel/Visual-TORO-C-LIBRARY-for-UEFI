/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _ui64tow.c

Abstract:

    Implementation of the Microsoft C function.
    Converts an integer to a string.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <limits.h>

extern int swprintf(wchar_t* pszDest, size_t dwCount, const wchar_t* pszFormat, ...);

/**

Synopsis
    #include <stdlib.h>
    char* _ui64tow(int value, char* str, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/itoa-itow?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/itoa-itow?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/itoa-itow?view=msvc-160#return-value
**/
wchar_t* _ui64tow(long long value, wchar_t* str, int base)
{

    wchar_t format[16] = { L"%ll`000b" };    //'B' signed, 'b' unsigned
    wchar_t* p = { L"%llu" };

    if (base != 10)
    {
        format[6] = '0' + base % 10;
        format[5] = '0' + (base /= 10) % 10;
        format[4] = '0' + (base /= 10) % 10;

        p = &format[0];
    }
    swprintf(str, INT_MAX, p, value);
    return str;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _itoaCDEABI.c

Abstract:

    Import Library version

    Implementation of the Microsoft C function.
    Converts an integer to a string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) int sprintf(char* pszBuffer, const char* pszFormat, ...);

/**

Synopsis
    #include <stdlib.h>
    char* _itoa(int value, char* str, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/itoa-itow?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/itoa-itow?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/itoa-itow?view=msvc-160#return-value
**/
static char* _itoaCDEABI(int value, char* str, int base)
{

    char format[16] = { "%l`000b" };
    char* p = { "%d" };

    if (base != 10)
    {
        format[5] = '0' + base % 10;
        format[4] = '0' + (base /= 10) % 10;
        format[3] = '0' + (base /= 10) % 10;

        p = &format[0];
    }
    sprintf(str, p, value);
    return str;
}

MKCDEABI(_itoa);
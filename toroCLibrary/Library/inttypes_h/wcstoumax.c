/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wcstoumax.c

Abstract:

    Implementation of the Standard C function.
    Converts strings to an integer value of the largest supported unsigned integer type.

Author:

    Kilian Kegel

--*/
#include <stdint.h>
#include <wchar.h>

/**

Synopsis
    #include <inttypes.h>
    uintmax_t wcstoumax(const wchar_t* strSource,wchar_t** endptr,int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoumax-strtoumax-l-wcstoumax-wcstoumax-l
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=213
    The wcstoimax and wcstoumax functions are equivalent to the wcstol, wcstoll,
    wcstoul, and wcstoull functions except that the initial portion of the wide string is
    converted to intmax_t and uintmax_t representation, respectively.
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoumax-strtoumax-l-wcstoumax-wcstoumax-l#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoumax-strtoumax-l-wcstoumax-wcstoumax-l#return-value

**/
uintmax_t wcstoumax(const wchar_t* strSource,wchar_t** endptr,int base)
{
    return wcstoull(strSource, endptr, base);
}
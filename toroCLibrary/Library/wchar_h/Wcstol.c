/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wcstol.c

Abstract:

    Implementation of the Standard C function.
    Convert a wide string to an long-integer value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <limits.h>

extern int _cdeSwscanf(const wchar_t* pwcsBuffer, wchar_t** endptr, const wchar_t* pwcsFormat, ...);

/**

Synopsis
    #include <wchar.h>
    long wcstol(const wchar_t* string, wchar_t** end_ptr, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-160&viewFallbackFrom=vs-2019
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=387
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-160#return-value

**/
long wcstol(const wchar_t* s, wchar_t** endptr, int base) {
    long long l = 0;        // NOTE: initialize to 0 because in error case 0 is returned
    long lx = LONG_MAX;
    long ln = LONG_MIN;
    long long max = lx;
    long long min = ln;
    wchar_t Format[] = { L"%ll`00b" };

    Format[5] = (base /= 1) % 10 + '0';
    Format[4] = (base /= 10) % 10 + '0';

    _cdeSwscanf(s, endptr, Format, &l);

    return (long)(l > 0 ? (max < l ? max : l) : (min > l ? min : l));
}
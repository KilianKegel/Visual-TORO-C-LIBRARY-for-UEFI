/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wcstoull.c

Abstract:

    Implementation of the Standard C function.
    Converts a string to a long long value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern int _cdeSwscanf(const wchar_t* pwcsBuffer, wchar_t** endptr, const wchar_t* pwcsFormat, ...);

/**

Synopsis
    #include <wchar.h>
    unsigned long long wcstoll(const wchar_t *strSource,wchar_t **endptr,int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoll-strtoll-l-wcstoll-wcstoll-l
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=387
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoll-strtoll-l-wcstoll-wcstoll-l#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoll-strtoll-l-wcstoll-wcstoll-l#return-value

**/
unsigned long long wcstoull(const wchar_t* s, wchar_t** endptr, int base) {
    unsigned long long l = 0;           // NOTE: initialize to 0 because in error case 0 is returned
    wchar_t Format[] = { L"%ll`00b" };  // NOTE: 'B' signed, 'b' unsigned

    Format[5] = (base /= 1) % 10 + '0';
    Format[4] = (base /= 10) % 10 + '0';

    _cdeSwscanf((wchar_t*)s, endptr, &Format[0], &l);

    return l;
}
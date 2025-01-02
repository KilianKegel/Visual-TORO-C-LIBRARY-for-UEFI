/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    strtoll.c

Abstract:

    Implementation of the Standard C function.
    Convert strings to an long-integer value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern int _cdeSscanf(const char* pszBuffer, char** endptr, const char* pszFormat, ...);

/**

Synopsis
    #include <stdlib.h>
    long long strtoll(const char *strSource, char **endptr, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoll-strtoll-l-wcstoll-wcstoll-l?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoll-strtoll-l-wcstoll-wcstoll-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoll-strtoll-l-wcstoll-wcstoll-l?view=msvc-160#return-value

**/
long long strtoll(const char* s, char** endptr, int base) {
    long long l = 0;                // NOTE: initialize to 0 because in error case 0 is returned
    char Format[] = { "%ll`00B" };  // NOTE: 'B' signed, 'b' unsigned

    Format[5] = (base /= 1) % 10 + '0';
    Format[4] = (base /= 10) % 10 + '0';

    _cdeSscanf((char*)s, endptr, Format, &l);

    return l;
}
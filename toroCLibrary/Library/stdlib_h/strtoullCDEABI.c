/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    strtoullCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Converts a string to a long long value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern int _cdeSscanf(const char* pszBuffer, char** endptr, const char* pszFormat, ...);

/**

Synopsis
    #include <stdlib.h>
    long long strtoull(const char *strSource, char **endptr, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoull-strtoull-l-wcstoll-wcstoll-l?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoull-strtoull-l-wcstoll-wcstoll-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoull-strtoull-l-wcstoll-wcstoll-l?view=msvc-160#return-value

**/
static unsigned long long strtoullCDEABI(const char* s, char** endptr, int base) {
    unsigned long long l = 0;       // NOTE: initialize to 0 because in error case 0 is returned
    char Format[] = { "%ll`00b" };  // NOTE: 'B' signed, 'b' unsigned

    Format[5] = (base /= 1) % 10 + '0';
    Format[4] = (base /= 10) % 10 + '0';

    _cdeSscanf((char*)s, endptr, Format, &l);

    return l;
}

MKCDEABI(strtoull);
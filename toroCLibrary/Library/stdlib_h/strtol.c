/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    strtol.c

Abstract:

    Implementation of the Standard C function.
    Convert strings to an long-integer value.

Author:

    Kilian Kegel

--*/
#include <limits.h>
extern void* __cdeGetAppIf(void);
extern int _cdeSscanf(const char* pszBuffer, char** endptr, const char* pszFormat, ...);

/**

Synopsis
    #include <stdlib.h>
    unsigned long strtoul(const char *strSource, char **endptr, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-160&viewFallbackFrom=vs-2019
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-160#return-value

**/
long strtol(const char* s, char** endptr, int base) {
    long long l = 0;        // NOTE: initialize to 0 because in error case 0 is returned
    long lx = LONG_MAX;
    long ln = LONG_MIN;
    long long max = lx;
    long long min = ln;
    char Format[] = { "%ll`00b" };

    Format[5] = (base /= 1) % 10 + '0';
    Format[4] = (base /= 10) % 10 + '0';

    _cdeSscanf((char*)s, endptr, Format, &l);

    return (long)(l > 0 ? (max < l ? max : l) : (min > l ? min : l));
}
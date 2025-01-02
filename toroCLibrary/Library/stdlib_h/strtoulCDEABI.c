/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    strtoulCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Convert strings to an unsigned long-integer value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <limits.h>

extern int _cdeSscanf(const char* pszBuffer, char** endptr, const char* pszFormat, ...);

/**

Synopsis
    #include <stdlib.h>
    unsigned long strtoul(const char *strSource, char **endptr, int base);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoul-strtoul-l-wcstoul-wcstoul-l?view=msvc-160&viewFallbackFrom=vs-2019
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoul-strtoul-l-wcstoul-wcstoul-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoul-strtoul-l-wcstoul-wcstoul-l?view=msvc-160#return-value

**/
static unsigned long strtoulCDEABI(const char* s, char** endptr, int base) {
    unsigned long long l = 0;           // NOTE: initialize to 0 because in error case 0 is returned
    long long* pl = (long long *)&l;
    char Format[] = { "%ll`00b" };

    Format[5] = (base /= 1) % 10 + '0';
    Format[4] = (base /= 10) % 10 + '0';

    _cdeSscanf((char*)s, endptr, Format, &l);

    return (unsigned long)(*pl <= -4294967296LL || *pl >= +4294967296LL ? ULONG_MAX : l);
}

MKCDEABI(strtoul);
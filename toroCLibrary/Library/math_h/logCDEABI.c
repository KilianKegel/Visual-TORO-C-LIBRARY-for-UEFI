/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    log.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates natural logarithm (base e == 2.71828183)

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// ERRNO.H
//
#define EDOM    33
#define ERANGE  34
#define errno   (*_errno())
extern __declspec(dllimport) int* _errno(void);

extern double __cdecl __cde80387FYL2X(double x, double y);
extern double __cdeLN2;

/**

Synopsis
    #include <math.h>
    double log(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/log-logf-log10-log10f
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/log-logf-log10-log10f#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/log-logf-log10-log10f#return-value

    ln(x) = ln(2) * ld(x)       --> ln logarithmus naturalis, ld logarithmus dualis

**/
static double __cdecl logCDEABI(double x)
{   
    CDEDOUBLE d = { .dbl = x };
    CDEDOUBLE dRet;

    dRet.dbl = __cde80387FYL2X(x, __cdeLN2);

    if (0xFFF8000000000000ULL == dRet.uint64)
    {
        if (0xFFF8000000000000ULL != d.uint64)
        {
            errno = EDOM;
        }
    }

    if (0xFFF0000000000000ULL == dRet.uint64)
        errno = ERANGE;

    return dRet.dbl;

}

MKCDEABI(log);
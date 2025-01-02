/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    tan.c

Abstract:

    Implementation of the Standard C function.
    Calculates natural logarithm (base e == 2.71828183)

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

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
double __cdecl log(double d)
{   
    double dRet= __cde80387FYL2X(d, __cdeLN2);

    return dRet;

}
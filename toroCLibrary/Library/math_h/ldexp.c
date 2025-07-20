/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ldexp.c

Abstract:

    Implementation of the Standard C function.
    Multiplies a floating-point number by an integral power of two.

Author:

    Kilian Kegel

--*/
#undef NCDETRACE
#include <CdeServices.h>
#include <cde.h>

extern double __cde80387FSCALE(double x, double y);

/**

Synopsis
    #include <math.h>
    double ldexp(double x, int exp);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ldexp
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ldexp#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ldexp#return-value

     NOTE: SUBNORMAL: expb := 0
                      exp  := -1023
           INF:       expb := 7FF
                      exp  := +1024
**/

double __cdecl ldexp(double x, int exp)
{
    return __cde80387FSCALE(x, (double)exp);
}

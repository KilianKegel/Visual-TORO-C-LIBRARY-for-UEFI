/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    tan.c

Abstract:

    Implementation of the Standard C function.
    Calculates the tangens hyperbolicus of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <math.h>

/**

Synopsis
    #include <math.h>
    double tanh(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl
    https://en.wikipedia.org/wiki/Hyperbolic_functions
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl#return-value
                                2x
                sinh(x)        e   - 1
    tanh(x) = ------------ = ------------
                cosh(x)         2x
                               e   + 1
**/
double __cdecl tanh(double d)
{
    //CDEDOUBLE* pdbl = (void*)&d;
    //uint64_t di = 0x7FF8042000000000LL;
    //double* pd = (void*)&di;
    double epow2x = exp(2.0 * d);
    double epow2xm1 = epow2x - 1.0;
    double epow2xp1 = epow2x + 1.0;
    double dRet = epow2xm1 / epow2xp1;

    return dRet;
}
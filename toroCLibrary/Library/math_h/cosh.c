/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    cosh.c

Abstract:

    Implementation of the Standard C function.
    Calculates the hyperbolic cosine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <math.h>
#include <errno.h>
#include <CdeServices.h>
/**

Synopsis
    #include <math.h>
    double cosh(double);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cosh-coshf-coshl
    https://en.wikipedia.org/wiki/Hyperbolic_functions
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cosh-coshf-coshl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cosh-coshf-coshl#return-value

    cosh(x) = 0.5 * (e^x + e^(-x))

**/
double cosh(double x)
{
    double dRet = INFINITY;
    CDEDOUBLE d = { .dbl = x };

    do
    {
        //
        // range check / range results
        //
        if (0x7FF0000000000000ULL < d.uint64 && 0x8000000000000000ULL > d.uint64)
        {
            d.uint64 |= 0x0008000000000000;  // set to +QNAN
            dRet = d.dbl;                    // return x
            break;
        }

        if (0xFFF0000000000000ULL < d.uint64)
        {
            d.uint64 |= 0x0008000000000000;  // set to -QNAN
            dRet = d.dbl;                    // return x
            break;
        }

        if (0xC090000000000000ULL <= d.uint64)
        {
            d.dbl = INFINITY;                // set to +INF
            dRet = d.dbl;                    // return x
            break;
        }

        //
        // math calculation
        //
        if (1)
        {
            double epowx = exp(x);  // epoxw -> e power x

            if (0.0 == epowx)
                break;

            double epowminusx = exp(-x);

            if (0.0 == epowminusx)
                break;

            double sum = epowx + epowminusx;

            dRet = 0.5 * sum;
        }

    }while(0);

    if (INFINITY == dRet)
        if (INFINITY != x && -INFINITY != x)
            errno = ERANGE;

    return dRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    cosh.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the hyperbolic cosine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <errno.h>
#include <CdeServices.h>

extern __declspec(dllimport) double exp(double);

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
static double coshCDEABI(double x)
{
    double dRet = /*INFINITY*/0x7FF0000000000000ULL;
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
            d.dbl = /*INFINITY*/0x7FF0000000000000ULL;                // set to +INF
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

    if (/*INFINITY*/0x7FF0000000000000ULL == dRet)
        if (/*INFINITY*/0x7FF0000000000000ULL != x && /*-INFINITY*/ 0xFFF0000000000000ULL != x)
            errno = ERANGE;

    return dRet;
}

MKCDEABI(cosh);
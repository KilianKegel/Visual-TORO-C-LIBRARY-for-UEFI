/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    sinh.c

Abstract:

    Implementation of the Standard C function.
    Calculates the hyperbolic tangent of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <math.h>

/**

Synopsis
    #include <math.h>
    double sinh(double);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sinh-sinhf-sinhl
    https://en.wikipedia.org/wiki/Hyperbolic_functions
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sinh-sinhf-sinhl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sinh-sinhf-sinhl#return-value

    sinh(x) = 0.5 * (e^x - e^(-x))

**/
double sinh(double d)
{
    double epowx = exp(d);
    double epowminusx = exp(-d);
    double diff = epowx - epowminusx;

    return 0.5 * diff;
}
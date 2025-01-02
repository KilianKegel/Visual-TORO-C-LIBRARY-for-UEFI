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
double cosh(double d)
{
    double epowx = exp(d);
    double epowminusx = exp(-d);
    double sum = epowx + epowminusx;

    return 0.5 * sum;
}
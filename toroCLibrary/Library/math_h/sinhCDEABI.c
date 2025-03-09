/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    sinh.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the hyperbolic sine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) double exp(double);

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
static double sinhCDEABI(double d)
{
    double epowx = exp(d);
    double epowminusx = exp(-d);
    double diff = epowx - epowminusx;

    return 0.5 * diff;
}

MKCDEABI(sinh);
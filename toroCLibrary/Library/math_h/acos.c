/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    acos.c

Abstract:

    Implementation of the Standard C function.
    Calculates the arccosine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <stdint.h>

extern const double __cdePI_2;
extern double __cdecl asin(double x);

/**

Synopsis
    #include <math.h>
    double acos(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/acos-acosf-acosl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/acos-acosf-acosl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/acos-acosf-acosl#return-value

    arccos(x) = pi/2 - arcsin(x)

**/
double __cdecl acos(double x)
{
    return __cdePI_2 - asin(x);
}
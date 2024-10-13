/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    floor.c

Abstract:

    Implementation of the Standard C function.
    Calculates the ceiling of a value.

Author:

    Kilian Kegel

--*/
#include <stdint.h>

extern double modf(double x, double* intptr);

/**

Synopsis
    #include <math.h>
    double floor(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floor-floorf-floorl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floor-floorf-floorl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floor-floorf-floorl#return-value

**/
double __cdecl floor(double d)
{
    double dRet;
    double fract = modf(d, &dRet);
    double add = 0.0 == fract ? 0.0 : (d > 0.0 ? +0.0 : -1.0);
    
    return dRet + add;
}
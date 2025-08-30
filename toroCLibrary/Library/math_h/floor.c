/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    floor.c

Abstract:

    Implementation of the Standard C function.
    Calculates the floor of a value.

Author:

    Kilian Kegel

--*/

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
double __cdecl floor(double x)
{
    double dRet = x;

    if (-0.0 != x)
    {
        double fract = modf(x, &dRet);
        double add = 0.0 == fract ? 0.0 : (x > 0.0 ? +0.0 : -1.0);

        dRet = dRet + add;
    }
    return dRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    floor.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the floor of a value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) double modf(double x, double* intptr);

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
static double __cdecl floorCDEABI(double d)
{
    double dRet;
    double fract = modf(d, &dRet);
    double add = 0.0 == fract ? 0.0 : (d > 0.0 ? +0.0 : -1.0);
    
    return dRet + add;
}

MKCDEABI(floor);
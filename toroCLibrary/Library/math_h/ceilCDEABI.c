/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ceil.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the ceiling of a value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// math.h
//
#define signbit(_Val)   _dsign(_Val)

extern int _dsign(double d);
extern __declspec(dllimport) double modf(double x, double* intptr);

/**

Synopsis
    #include <math.h>
    double ceil(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ceil-ceilf-ceill
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ceil-ceilf-ceill#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ceil-ceilf-ceill#return-value

**/
static double __cdecl ceilCDEABI(double d)
{
    double dRet;
    double fract = modf(d, &dRet);
    double add = 0.0 == fract ? 0.0 : (d > 0.0 ? +1.0 : 0.0);
    
    dRet = dRet + add;
    if(0.0 == dRet && signbit(d))
		dRet = -0.0;
    return dRet;
}

MKCDEABI(ceil);
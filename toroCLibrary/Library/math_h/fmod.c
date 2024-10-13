/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    fmod.c

Abstract:

    Implementation of the Standard C function.
    Calculates the floating-point remainder.

Author:

    Kilian Kegel

--*/
#include <math.h>

extern double modf(double x, double* intptr);

/**

Synopsis
    #include <math.h>
    double fmod(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fmod-fmodf
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fmod-fmodf#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fmod-fmodf#return-value

**/
double __cdecl fmod(double x, double y)
{
    double dquotient = x / y;
    double iquotient;
    double dRet;
    modf(dquotient, &iquotient);
    
    dRet = x - iquotient * y;
    if (0.0 == dRet)
        dRet = signbit(x) ? -dRet : dRet;
    return dRet;
}
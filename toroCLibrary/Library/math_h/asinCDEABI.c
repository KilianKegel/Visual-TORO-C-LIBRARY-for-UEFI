/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    asin.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the arcsine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) double __cdecl sqrt(double x);
extern __declspec(dllimport) double __cdecl atan(double x);

/**

Synopsis
    #include <math.h>
    double sin(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/asin-asinf-asinl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/asin-asinf-asinl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/asin-asinf-asinl#return-value

                        /       x        \
    arcsin(x) = arctan  |----------------|
                        \   √(1 - x²)    /

**/
static double __cdecl asinCDEABI(double x)
{
    double dRet = 0.0;
    double x2 = x * x;
    double oneminx2 = 1 - x2;
    double rootoneminx2 = sqrt(oneminx2);
    double xdivrootoneminx2 = x / rootoneminx2;

    dRet = atan(xdivrootoneminx2);

    return dRet;
}

MKCDEABI(asin);
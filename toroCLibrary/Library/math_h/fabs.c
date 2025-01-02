/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    fabs.c

Abstract:

    Implementation of the Standard C function.
    Calculates the absolute value of the floating-point argument.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <math.h>
    double fabs(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fabs-fabsf-fabsl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fabs-fabsf-fabsl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fabs-fabsf-fabsl#return-value

**/
double fabs(double x)
{
    CDEDOUBLE* pd = (void*) &x;

    pd->member.sign = 0;

    return pd->dbl;
}
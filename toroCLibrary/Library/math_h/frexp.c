/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    frexp.c

Abstract:

    Implementation of the Standard C function.
    Gets the mantissa and exponent of a floating-point number.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern double modf(double x, double* intptr);

/**

Synopsis
    #include <math.h>
    double frexp(double x, int *expptr);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/frexp
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/frexp#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/frexp#return-value

**/
double __cdecl frexp(double x, int* expptr)
{
    CDEDOUBLE dbl = { .dbl = x };
    int exp = (int)dbl.member.exp - 1023;

    dbl.member.exp = exp == 1024 ? -1 : 1 + dbl.member.exp;
    dbl.member.mant = 0;
    dbl.member.sign = 0;

    *expptr = exp + 1 > 1024 ? -1 : exp + 1;
    return x / dbl.dbl;
}
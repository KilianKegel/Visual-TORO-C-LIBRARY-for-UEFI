/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    sqrt.c

Abstract:

    Implementation of the Standard C function.
    Calculates the square root of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern double __cdecl __cde80387FSQRT(double x);

/**

Synopsis
    #include <math.h>
    double modf(double x, double* intptr);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl#return-value

**/
double __cdecl sqrt(double d)
{
    CDEDOUBLE* pdbl = (void*)&d;
    uint64_t di = 0xFFF8000000000000ULL;// 0x7FF8002000000000LL;//DOMAIN ERROR
    double* pd = (void*)&di;

    do
    {
        if (0 == pdbl->member.exp)      // 0.0?
        {
            *pd = d;                    // return 0.0
            break;
        }

        if (0 == pdbl->member.sign)     // non-neg?
        {
            *pd = __cde80387FSQRT(d);
        }

    } while (0);

    return *pd;
}
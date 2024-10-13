/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    sin.c

Abstract:

    Implementation of the Standard C function.
    Calculates the sine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern double __cdecl __cde80387FSIN(double x);

/**

Synopsis
    #include <math.h>
    double sin(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sin-sinf-sinl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sin-sinf-sinl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sin-sinf-sinl#return-value

**/
double __cdecl sin(double d)
{
    CDEDOUBLE* pdbl = (void*)&d;
    uint64_t di = 0x7FF8042000000000LL;
    double* pd = (void*)&di;

    if ((63 + 1023) > pdbl->member.exp)
    {
        *pd = __cde80387FSIN(d);
    }

    return *pd;
}
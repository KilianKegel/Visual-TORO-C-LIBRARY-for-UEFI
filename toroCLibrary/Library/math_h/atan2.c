/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    atan2.c

Abstract:

    Implementation of the Standard C function.
    Calculates the arcatangent of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern double __cdecl __cde80387FPATAN(double x);
extern const double __cdePI;
extern const double __cdePI_2;

/**

Synopsis
    #include <math.h>
    double atan2(double x, double y);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l#return-value

**/
double __cdecl atan2(double y, double x)
{
    double d = 0.0 == x ? 0.0 : y / x;
    uint64_t di = 0xFFF8000000000000ULL;// 0x7FF8002000000000LL;//DOMAIN ERROR
    double* pd = (void*)&di;

    do
    {
        if (0.0 != x)
        {
            d = __cde80387FPATAN(d);

            if (0.0 > x) {
                if (0.0 <= y)
                    d = d + __cdePI;
                else if(0.0 > y)
                    d = d - __cdePI;
            }
        } else {
            if (0.0 < y)
                d = d + __cdePI_2;
            else if (0.0 > y)
                d = d - __cdePI_2;
        }
        *pd = d;
    } while (0);

    return *pd;
}


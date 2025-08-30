/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    atan2.c

Abstract:

    Implementation of the Standard C function.
    Calculates the arcatangent of y/x floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <math.h>
#include <errno.h>

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
    CDEDOUBLE* pd = (void*)&d;
    CDEDOUBLE* py = (void*)&y;
    CDEDOUBLE* px = (void*)&x;
    char fYSNAN= ((0x7FF == py->member.exp) && !(0x0008000000000000ULL & py->member.mant) && (0x0007FFFFFFFFFFFFULL & py->member.mant));
    char fYNAN = ((0x7FF == py->member.exp) && (0x0008000000000000ULL & py->member.mant));
    char fYZERO= ((0x000 == py->member.exp) && (0 == py->member.mant));

    char fXSNAN= ((0x7FF == px->member.exp) && !(0x0008000000000000ULL & px->member.mant) && (0x0007FFFFFFFFFFFFULL & px->member.mant));
    char fXNAN = ((0x7FF == px->member.exp) && (0x0008000000000000ULL & px->member.mant));
    char fXZERO= ((0x000 == px->member.exp) && (0 == px->member.mant));

    if ((fYNAN || fYSNAN) && (fXNAN || fXSNAN)) // check for non-NAN
        d = x,
        pd->uint64 |= 0x0008000000000000; // MSFT unified NAN 
    else
    {
        //
        // W/A for atan2(+-INF, +-INF)
        //
        if (y == x)
            d = 1.0;
        else if (-y == +x)
            d = -1.0;
        else if (+y == -x)
            d = -1.0;

        if (!fXZERO)
        {
            d = __cde80387FPATAN(d);

            if (0.0 > x) {
                if (+0.0 == y || -0.0 == y) {
                    if (_dsign(y))
                        d = _dsign(x) ? -__cdePI : +__cdePI;
                    else
                        d = _dsign(x) ? +__cdePI : -__cdePI;
                }
                else if (0.0 < y)
                    d = d + __cdePI;
                else if (0.0 > y)
                    d = d - __cdePI;
            }
        }
        else {
            if (0.0 < y)
                d = d + __cdePI_2;
            else if (0.0 > y)
                d = d - __cdePI_2;
            else if (!fYZERO)
                pd->uint64 = 0x0008000000000000ULL | py->uint64;
            else if (0 == px->member.sign && 0 == py->member.sign) // x== +0, y == +0
                d = +0.0;
            else if (0 == px->member.sign && 0 != py->member.sign) // x== +0, y == -0
                d = -0.0;
            else if (0 != px->member.sign && 0 != py->member.sign) // x== -0, y == -0
                d = -__cdePI;
            else if (0 != px->member.sign && 0 == py->member.sign) // x== -0, y == +0
                d = +__cdePI;
        }

    }

    return d;
}


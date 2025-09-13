/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    sinh.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the hyperbolic sine of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
extern void abort(void);
extern __declspec(dllimport) double exp(double);
//
// ERRNO.H
//
#define EDOM    33
#define ERANGE  34
#define errno   (*_errno())
extern __declspec(dllimport) int* _errno(void);

/**

Synopsis
    #include <math.h>
    double sinh(double);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sinh-sinhf-sinhl
    https://en.wikipedia.org/wiki/Hyperbolic_functions
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sinh-sinhf-sinhl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sinh-sinhf-sinhl#return-value

    sinh(x) = 0.5 * (e^x - e^(-x))

**/
static double sinhCDEABI(double x)
{
    CDEDOUBLE d = { .dbl = x };
    CDEDOUBLE dRet = { .dbl = x };
    double epowx;
    double epowminusx;
    double diff;
    do
    {
        if (-0.0 == x || +0.0 == x) {
            dRet.dbl = x;
            break;
        }

        if (1 == d.member.sign)
        {
            if (0xBE54000000000000ULL >= d.uint64)
                break;  // return x
        }
        else {
            if (d.member.exp <= 0x3E4)    // positive number
            {
                if (d.member.sign == 0)
                    dRet.dbl = x;
                else
                    dRet.dbl = -x;
                break;          // return +/-0.0
            }
        }


        epowx = exp(d.dbl);
        epowminusx = exp(-d.dbl);
        diff = epowx - epowminusx;

        dRet.dbl = 0.5 * diff;

    } while (0);

    if (0x7FFULL == dRet.member.exp)
        if (0ULL == dRet.member.mant)
            if(0x7FF0000000000000ULL != d.uint64 && 0xFFF0000000000000ULL != d.uint64)
                errno = ERANGE;                     // set domain error

    return dRet.dbl;
}

MKCDEABI(sinh);
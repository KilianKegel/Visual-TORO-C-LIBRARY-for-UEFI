/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    tan.c

Abstract:

    Implementation of the Standard C function.
    Calculates the tangens of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <errno.h>
extern double __cdecl __cde80387FPTAN(double x);

/**

Synopsis
    #include <math.h>
    double tan(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl#return-value

**/
double __cdecl tan(double x)
{
    CDEDOUBLE dRet = { .dbl = x };

    do
    {
        if (0x7FFULL == dRet.member.exp)
        {
            if(0ULL == dRet.member.mant)
                dRet.uint64 = 0x8008000000000000ULL | dRet.uint64,
                errno = EDOM;
            else
                dRet.uint64 = 0x0008000000000000ULL | dRet.uint64;
            
            break;
        }

        if ((63 + 1023) > dRet.member.exp)
            dRet.dbl = __cde80387FPTAN(x);
        else
            dRet.uint64 = 0x7FF8042000000000LL,
            errno = ERANGE;

    } while (0);

    return dRet.dbl;
}
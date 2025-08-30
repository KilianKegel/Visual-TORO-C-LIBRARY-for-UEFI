/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    tan.c

Abstract:

    Implementation of the Standard C function.
    Calculates the tangens hyperbolicus of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <math.h>
#include <CdeServices.h>
/**

Synopsis
    #include <math.h>
    double tanh(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl
    https://en.wikipedia.org/wiki/Hyperbolic_functions
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/tan-tanf-tanl#return-value
                                2x
                sinh(x)        e   - 1
    tanh(x) = ------------ = ------------
                cosh(x)         2x
                               e   + 1
**/
double __cdecl tanh(double x)
{
    CDEDOUBLE dRet = { .dbl = x };
    double epow2x;
    double epow2xm1;
    double epow2xp1;

    do
    {
        if (dRet.member.exp < 0x3E5)
            break;
        if (dRet.member.exp <= 0x7FE && dRet.member.exp >= 0x407)
        {
            if(0 == dRet.member.sign)
                dRet.dbl = +1.0;
            else
                dRet.dbl = -1.0;
            break;
        }
        
        if (dRet.uint64 == 0x7FF0000000000000ULL)
        {
            dRet.dbl = +1.0;
            break;
        }

        epow2x = exp(2.0 * x);
        epow2xm1 = epow2x - 1.0;
        epow2xp1 = epow2x + 1.0;
        dRet.dbl = epow2xm1 / epow2xp1;

    } while (0);
    
    return dRet.dbl;
}
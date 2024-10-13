/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ldexp.c

Abstract:

    Implementation of the Standard C function.
    Multiplies a floating-point number by an integral power of two.

Author:

    Kilian Kegel

--*/
#undef NCDETRACE
#include <CDE.h>
#include <CdeServices.h>

/**

Synopsis
    #include <math.h>
    double ldexp(double x, int exp);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ldexp
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ldexp#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/ldexp#return-value

     NOTE: SUBNORMAL: expb := 0
                      exp  := -1023
           INF:       expb := 7FF
                      exp  := +1024
**/
double __cdecl ldexp(double x, int exp)
{
#define BIAS 1023
    CDEDOUBLE fact = { .uint64 = 0LL }, prod;
    CDEDOUBLE* px = (void*)&x;
    int expx/*exp of x*/ = (int)px->member.exp - BIAS;

    //
    // check subnormals
    //
    if (exp <= -1023) 
    {
        int mantRSHIFT = -1 * (1023 + exp);;
        fact.member.mant = (1ULL << (CDEDOUBLE_EXPPOS - 1)) >> mantRSHIFT;

        if (exp <= -1023 - 52)
        {
            fact.member.mant = 1ULL;
            x = x * (-exp - 1023 - 51);
            x = x * 0.5;
        }
    } else {
        if ((exp + expx) <= 1023/* result */  && (exp > 1023)/* parm */ && x != 0.0)
        {
            px->member.exp += (exp - 1023);
            exp = 1023;
        }
        fact.member.exp = exp + 1023;
    }
    

    if (exp > 1024)
        fact.uint64 = 0x7FF0000000000000ULL;

    if(exp == 1024 && (1.0 > x || -1.0 < x))
        fact.member.exp = 2046,
        x = x * 2.0;

    if (0.0 == x)
        fact.uint64 = 0;

    prod.dbl = x * fact.dbl;

    return prod.dbl;

}
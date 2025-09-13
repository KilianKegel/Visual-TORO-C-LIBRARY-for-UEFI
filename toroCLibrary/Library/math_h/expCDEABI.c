/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    exp.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the exponential of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// ERRNO.H
//
#define EDOM    33
#define ERANGE  34
#define errno   (*_errno())
extern __declspec(dllimport) int* _errno(void);

//
// math.h
//
#define signbit(_Val)   _dsign(_Val)
#define _HUGE_ENUF      1e+300
#define INFINITY        ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define NAN             (-(float)(INFINITY * 0.0F))

extern const double __cdeLOG2E;
extern __declspec(dllimport) double modf(double, double*);
extern double __cde80387F2XM1(double);

/**

Synopsis
    #include <math.h>
    double exp(double);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/exp-expf
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/exp-expf#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/exp-expf#return-value

         d    d*ld(e)
        e  = 2       

            dxlde         == d*ld(e)    --> ld() logarithmus dualis

                              intgr(dxlde)
            twoPowerIntgr == 2            
        
                              fract(dxlde)
            twoPowerFract == 2

             d    intgr(dxlde)     fract(dxlde)
        --> e  = 2              * 2

             d
        --> e  = twoPowerIntgr * twoPowerFract
                                                           x
    NOTE:   80387's only exponential function is F2XM1 -> 2  - 1 in the range -1 <= x <= + 1
            
**/
static double expCDEABI(double x)
{
    double dRet = INFINITY;
    double intgr, fract;
    double twoPowerFract = 0.0;
    double twoPowerIntgr = 0.0;
    CDEDOUBLE* ptwoPowerIntgr = (void*) &twoPowerIntgr;
    double dxlde = x * __cdeLOG2E;  // product of x and ld(e)

    do {

        if (INFINITY == dxlde)
            break;

        if (-INFINITY == dxlde) {
            dRet = 0.0;
            break;
        }

        fract = modf(dxlde, &intgr);

        twoPowerFract = 1.0 + __cde80387F2XM1(fract);

        if (intgr > -1023)                                      // deal with normalized doubles
        {
            if (0x7FEULL < 1023/*bias*/ + (uint64_t)intgr)      // check exponent overflow
                break;  // return with error

            ptwoPowerIntgr->member.exp = 1023/*bias*/ + (uint64_t)intgr;
        }
        else                                                    // deal with subnormals
        {
            // -1023 -> (1 << 51)
            // -1024 -> (1 << 50)
            // ...
            // -1074 -> (1 <<  0)
            if (((int64_t)intgr + 1023 + 51) >= 0)
                ptwoPowerIntgr->member.mant = 1ULL << ((int64_t)intgr + 1023 + 51);
        }
        
        dRet = twoPowerIntgr * twoPowerFract;

    } while (0);

    if (INFINITY == dRet)
        if (INFINITY != x && -INFINITY != x)
            errno = ERANGE;

    return dRet;
}

MKCDEABI(exp);
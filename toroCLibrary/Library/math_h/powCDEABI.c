/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    pow.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates x raised to the power of y.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// math.h
//
#define signbit(_Val)   _dsign(_Val)
#define _HUGE_ENUF      1e+300
#define INFINITY        ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define NAN             (-(float)(INFINITY * 0.0F))

extern double __cde80387F2XM1(double);
extern double __cde80387FYL2X(double x, double y);
extern __declspec(dllimport) double exp(double);
extern __declspec(dllimport) double modf(double, double*);
extern __declspec(dllimport) double fabs(double x);
extern __declspec(dllimport) double fmod(double x, double y);
extern int _dsign(double d);


/**

Synopsis
    #include <math.h>
    double pow(double);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/pow-powf-powl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/pow-powf-powl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/pow-powf-powl#return-value

         y    y*ld(x)
        x  = 2

            yxldx         == y*ld(x)    --> ld() logarithmus dualis

                              intgr(yxldx)
            twoPowerIntgr == 2

                              fract(yxldx)
            twoPowerFract == 2

             y    intgr(yxldx)     fract(yxldx)
        --> x  = 2              * 2

             y
        --> x  = twoPowerIntgr * twoPowerFract


                                                           x
    NOTE:   80387's only exponential function is F2XM1 -> 2  - 1 in the range -1 <= x <= + 1

**/
static double powCDEABI(double bas, double exp)
{
    CDEDOUBLE product = { .dbl = -NAN };                                  // assume failure
    CDEDOUBLE x = { .dbl = bas };
    CDEDOUBLE y = { .dbl = exp };
    double signedOneX = 0 == signbit(x.dbl) ? 1.0 : -1.0;
    CDEDOUBLE absx = { .dbl = fabs(x.dbl) };
    CDEDOUBLE intgr, fract;
    CDEDOUBLE twoPowerFract = { .dbl = 0.0 };
    CDEDOUBLE twoPowerIntgr = { .dbl = 0.0 };
    double tmp;
    char fYIsFrational = 0.0 != modf(y.dbl, &tmp);
    char fYIsEven = 0 == fmod(y.dbl, 2.0);

    do
    {
        //
        // preprocess new
        //
        if (1)
        {
            if (fYIsEven && -1.0 == signedOneX)
                signedOneX = 1.0;

            if(0.0 == y.dbl || -0.0 == y.dbl)
            {
                if (0x7FF == x.member.exp) 
                {
                    if ((0 != x.member.mant) && (0x0008000000000000ULL > x.member.mant)) 
                    {
                        product.uint64 = 0x0008000000000000ULL | x.uint64;
                        break;
                    }
                }
            }

            if (0x7FF0000000000000ULL == y.uint64)    //  Y INF
            {

                if (1.0 == x.dbl || -1.0 == x.dbl)
                {
                    product.dbl = 1.0;
                    break;
                }

                if (1.0 > x.dbl && 0.0 < x.dbl)
                {
                    product.dbl = 0.0;
                    break;
                }
                
                //
                // X NEGATIVE
                //
                if (signbit(x.dbl)) {
                    if (0xBFEFFFFFFFFFFFFFULL >= x.uint64 && 0x8000000000000000ULL <= x.uint64) {
                        product.uint64 = 0x0000000000000000ULL;
                        break;
                    }
                    if (0xFFF0000000000000ULL >= x.uint64 && 0xBFF0000000000001ULL <= x.uint64) {
                        product.uint64 = 0x7FF0000000000000ULL;
                        break;
                    }
                }
            }

            if (0xFFF0000000000000ULL == y.uint64)    //  Y -INF
            {
                if (1.0 == x.dbl || -1.0 == x.dbl)
                {
                    product.dbl = 1.0;
                    break;
                }

                if (1.0 <= x.dbl && 0x7FF0000000000000ULL >= x.uint64)
                {
                    product.dbl = 0.0;
                    break;
                }

                if (-0.0 >= x.dbl && -1.0 < x.dbl)
                {
                    product.uint64 = 0x7FF0000000000000ULL;
                    break;
                }

                if (-1.0 > x.dbl && -INFINITY <= x.dbl)
                {
                    product.dbl = 0.0;
                    break;
                }
            }

            if(0x7FF == y.member.exp && 0ULL != y.member.mant)//  +-Y QSNAN
            {
                if (1.0 == x.dbl) 
                {
                    if (0x0008000000000000ULL & y.uint64) {
                        product.dbl = 1.0;
                        //else
                        //  product.uint64 = 0x0008000000000000ULL | y.uint64;
                        break;
                    }
                }

                if (0x7FF == x.member.exp && 0ULL != x.member.mant)
                {
                    product.uint64 = 0x0008000000000000ULL | x.uint64;
                    break;
                }
                product.uint64 = 0x0008000000000000ULL | y.uint64;
                break;
            }

            if (-INFINITY == x.dbl || INFINITY == x.dbl)
            {
                if (-INFINITY == x.dbl)
                {
                    if (-1.0 == y.dbl)
                    {
                        product.dbl = -0.0;
                        break;
                    }
                }

                if (0.0 < y.dbl && 1.0 != y.dbl && -INFINITY == x.dbl)
                {
                    if (fYIsFrational || fYIsEven)
                        product.dbl = INFINITY;
                    else
                        product.dbl = -INFINITY;
                    break;
                }

                if (-0.0 > y.dbl)   // Y NEGATIVE
                {
                    if(fYIsFrational || fYIsEven)
                        product.dbl = 0.0;
                    else
                        product.dbl = -0.0;
                    break;
                }
            }

            if (1.0 == fabs(y.dbl) && (0xFFF0000000000000ULL < x.uint64 && 0xFFFFFFFFFFFFFFFFULL >= x.uint64))
            {
                product.uint64 = 0x0008000000000000ULL | x.uint64;
                product.member.sign = 0;
                break;
            }

            if (-0.0 > x.dbl)
            {
                if (0.0 == fabs(y.dbl))
                {
                    product.dbl = 1.0;
                    break;
                }
                
                if (1.0 == y.dbl)
                {
                    product.dbl = x.dbl;
                    break;
                }

                if (fYIsFrational) 
                {
                    product.uint64 = 0xFFF8000000000000ULL;
                    break;
                }
            }
        }

        ////////////////////////////////////////////////
        // 80387 calculation
        ////////////////////////////////////////////////
        CDEDOUBLE yxldx = { .dbl = __cde80387FYL2X(absx.dbl, y.dbl) };  // product of exponent and ld(base)
        if (0x7FF == yxldx.member.exp)
        {
            if (0.0 == y.dbl)
                product.dbl = 1.0;
            else if (signbit(yxldx.dbl))
            {
                if (fYIsFrational)
                    if (0.0 == fabs(x.dbl))
                        product.dbl = +0.0;                     //001: pow(8000000000000000, 0008000000000000) -> FFF8000000000000
                    else
                        product.uint64 = 0xFFF8000000000000ULL; //401: pow(8000000000000001, 4070000000000001)
                else
                    if (fYIsEven)
                        product.dbl = +0.0;
                    else
                        if (0.0 == fabs(x.dbl))
                            product.dbl = x.dbl;
                        else if (1.0 == y.dbl)
                            product.dbl = x.dbl;
                        else
                            product.dbl = -0.0;
            }
            else {
                if (signbit(x.dbl))
                    product.dbl = -yxldx.dbl;
                else
                    product.dbl = +yxldx.dbl;

                if (0x7FF0000000000000ULL == yxldx.uint64) {
                    if (signbit(x.dbl))
                        if (fYIsFrational)
                            product.dbl = +yxldx.dbl;
                        else
                            if (fYIsEven)
                                product.dbl = +yxldx.dbl;
                            else
                                product.dbl = -yxldx.dbl;
                    else
                        product.dbl = +yxldx.dbl;
                }
                
                if (0ULL != x.member.mant)
                    if (signbit(product.dbl)) 
                    {
                        product.uint64 |= 0x0008000000000000ULL;
                        if (0x7FF != y.member.exp)
                        {
                            if (fYIsFrational || fYIsEven)
                                product.dbl = +product.dbl;
                            else
                                product.dbl = -product.dbl;
                        }
                    }
            }
            break;
        }
        fract.dbl = modf(yxldx.dbl, &intgr.dbl);

        twoPowerFract.dbl = 1.0 + __cde80387F2XM1(fract.dbl);

        if (intgr.dbl > -1023.0)  // deal with normalized doubles
        {
            int ntmp = (int)intgr.dbl + 1023/*bias*/;

            if (ntmp > 0x7FE || 1024.0 < intgr.dbl)
            {
                product.dbl = signedOneX * INFINITY;
                break;
            }
            twoPowerIntgr.member.exp = 1023/*bias*/ + (uint64_t)intgr.dbl;
        }
        else        // deal with subnormals
        {
            // -1023 -> (1 << 51)
            // -1024 -> (1 << 50)
            // ...
            // -1074 -> (1 <<  0)
            if (((int64_t)intgr.dbl + 1023 + 51) >= 0)
                twoPowerIntgr.member.mant = 1ULL << ((int64_t)intgr.dbl + 1023 + 51);
        }

        product.dbl = signedOneX * twoPowerIntgr.dbl * twoPowerFract.dbl;

    } while (0);


    return product.dbl;
}

MKCDEABI(pow);
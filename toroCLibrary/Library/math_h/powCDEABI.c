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
    int currentERRNO = errno;

    int yxldxint = 0;
    CDEDOUBLE dRet = { .dbl = -NAN };                                   // assume failure
    CDEDOUBLE x = { .dbl = bas };
    CDEDOUBLE y = { .dbl = exp };
    double signedOneX = (0 == x.member.sign ? 1.0 : -1.0);
    CDEDOUBLE absx = { .dbl = fabs(x.dbl) };
    CDEDOUBLE intgr, fract;
    CDEDOUBLE twoPowerFract = { .dbl = 0.0 };
    CDEDOUBLE twoPowerIntgr = { .dbl = 0.0 };
    double tmp;
    char fYIsFrational = 0.0 != modf(y.dbl, &tmp);
    char fYIsEven = 0 == fmod(y.dbl, 2.0);
    errno = currentERRNO;                                               // restore errno drom fmod()

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
                        dRet.uint64 = 0x0008000000000000ULL | x.uint64;
                        break;
                    }
                }
            }

            if (0x7FF0000000000000ULL == y.uint64)    //  Y INF
            {
                if (1.0 == x.dbl || -1.0 == x.dbl)
                {
                    dRet.dbl = 1.0;
                    break;
                }

                if (1.0 > x.dbl && 0.0 < x.dbl)
                {
                    dRet.dbl = 0.0;
                    break;
                }
                
                //
                // X NEGATIVE
                //
                if (x.member.sign) {
                    if (0xBFEFFFFFFFFFFFFFULL >= x.uint64 && 0x8000000000000000ULL <= x.uint64) {
                        dRet.uint64 = 0x0000000000000000ULL;
                        break;
                    }
                    if (0xFFF0000000000000ULL >= x.uint64 && 0xBFF0000000000001ULL <= x.uint64) {
                        dRet.uint64 = 0x7FF0000000000000ULL;
                        //errno = EDOM;
                        break;
                    }
                }
            }

            if (0xFFF0000000000000ULL == y.uint64)    //  Y -INF
            {
                if (1.0 == x.dbl || -1.0 == x.dbl)
                {
                    dRet.dbl = 1.0;
                    break;
                }

                if (1.0 <= x.dbl && 0x7FF0000000000000ULL >= x.uint64)
                {
                    dRet.dbl = 0.0;
                    break;
                }

                if (-0.0 >= x.dbl && -1.0 < x.dbl)
                {
                    dRet.uint64 = 0x7FF0000000000000ULL;
                    break;
                }

                if (-1.0 > x.dbl && -INFINITY <= x.dbl)
                {
                    dRet.dbl = 0.0;
                    break;
                }
            }

            if(0x7FF == y.member.exp && 0ULL != y.member.mant)//  +-Y QSNAN
            {
                if (1.0 == x.dbl) 
                {
                    if (0x0008000000000000ULL & y.uint64) {
                        dRet.dbl = 1.0;
                        //else
                        //  dRet.uint64 = 0x0008000000000000ULL | y.uint64;
                        break;
                    }
                }

                if (0x7FF == x.member.exp && 0ULL != x.member.mant)
                {
                    dRet.uint64 = 0x0008000000000000ULL | x.uint64;
                    break;
                }
                dRet.uint64 = 0x0008000000000000ULL | y.uint64;
                break;
            }

            if (-INFINITY == x.dbl || INFINITY == x.dbl)
            {
                if (-INFINITY == x.dbl)
                {
                    if (-1.0 == y.dbl)
                    {
                        dRet.dbl = -0.0;
                        break;
                    }
                }

                if (0.0 < y.dbl && 1.0 != y.dbl && -INFINITY == x.dbl)
                {
                    if (fYIsFrational || fYIsEven)
                        dRet.dbl = INFINITY;
                    else
                        dRet.dbl = -INFINITY;
                    break;
                }

                if (-0.0 > y.dbl)   // Y NEGATIVE
                {
                    if(fYIsFrational || fYIsEven)
                        dRet.dbl = 0.0;
                    else
                        dRet.dbl = INFINITY == x.dbl ? +0.0 : -0.0;
                    break;
                }
            }

            if (1.0 == fabs(y.dbl) && (0xFFF0000000000000ULL < x.uint64 && 0xFFFFFFFFFFFFFFFFULL >= x.uint64))
            {
                dRet.uint64 = 0x0008000000000000ULL | x.uint64;
                dRet.member.sign = 0;
                break;
            }

            if (-0.0 > x.dbl)
            {
                if (0.0 == fabs(y.dbl))
                {
                    dRet.dbl = 1.0;
                    break;
                }
                
                if (1.0 == y.dbl)
                {
                    dRet.dbl = x.dbl;
                    break;
                }

                if (fYIsFrational) 
                {
                    dRet.uint64 = 0xFFF8000000000000ULL;
                    break;
                }
            }
        }

        ////////////////////////////////////////////////
        // 80387 calculation
        ////////////////////////////////////////////////
        CDEDOUBLE yxldx = { .dbl = __cde80387FYL2X(absx.dbl, y.dbl) };  // product of exponent and ld(base)
        yxldxint = (int)yxldx.dbl;                    // integer part of yxldx, Microsoft error limit is about -1477

        if (0x7FF == yxldx.member.exp)
        {
            if (0.0 == y.dbl)
                dRet.dbl = 1.0;
            else if (yxldx.member.sign)
            {
                if (fYIsFrational)
                    if (0.0 == fabs(x.dbl))
                        dRet.dbl = +0.0;                     //001: pow(8000000000000000, 0008000000000000) -> FFF8000000000000
                    else
                        dRet.uint64 = 0xFFF8000000000000ULL; //401: pow(8000000000000001, 4070000000000001)
                else
                    if (fYIsEven)
                        dRet.dbl = +0.0;
                    else
                        if (0.0 == fabs(x.dbl))
                            dRet.dbl = x.dbl;
                        else if (1.0 == y.dbl)
                            dRet.dbl = x.dbl;
                        else
                            dRet.dbl = -0.0;
            }
            else {
                if (x.member.sign)
                    dRet.dbl = -yxldx.dbl;
                else
                    dRet.dbl = +yxldx.dbl;

                if (0x7FF0000000000000ULL == yxldx.uint64) {
                    //kgtest dRet.uint64 = yxldx.uint64;
                    if (x.member.sign)
                        if (fYIsFrational)
                            dRet.dbl = +yxldx.dbl;
                        else
                            if (fYIsEven)
                                dRet.dbl = +yxldx.dbl;
                            else
                                dRet.dbl = -yxldx.dbl;
                    else
                        dRet.dbl = +yxldx.dbl;
                }
                
                if (0ULL != x.member.mant)
                    if (dRet.member.sign) 
                    {
                        dRet.uint64 |= 0x0008000000000000ULL;
                        if (0x7FF != y.member.exp)
                        {
                            if (fYIsFrational || fYIsEven)
                                dRet.dbl = +dRet.dbl;
                            else
                                dRet.dbl = -dRet.dbl;
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
                dRet.dbl = signedOneX * INFINITY;
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

        dRet.dbl = signedOneX * twoPowerIntgr.dbl * twoPowerFract.dbl;

    } while (0);


    //
    // errno
    //
    if (0xFFF8000000000000ULL == dRet.uint64
        && 0xFFF8000000000000ULL != x.uint64
        && 0xFFF8000000000000ULL != y.uint64)
        errno = EDOM;
    else if (0x7FF0000000000000ULL == dRet.uint64
        && !(0x7FF0000000000000ULL == y.uint64 || 0xFFF0000000000000ULL == y.uint64 || 0x7FF0000000000000ULL == x.uint64 || 0xFFF0000000000000ULL == x.uint64))
        errno = ERANGE;
    else if (0xFFF0000000000000ULL == dRet.uint64 && x.uint64 != 0xFFF0000000000000 && y.uint64 != 0x3FF0000000000000)
        errno = ERANGE;
    else if ((0.0 == dRet.dbl) && (0.0 != bas) && (0.0 == twoPowerIntgr.dbl) && (yxldxint >= -1477))
        if((0x7FF0000000000000ULL != (0x7FF0000000000000ULL & x.uint64)) && (0x7FF0000000000000ULL != ((0x7FF0000000000000ULL & y.uint64))))
            errno = ERANGE;

    return dRet.dbl;
}

MKCDEABI(pow);
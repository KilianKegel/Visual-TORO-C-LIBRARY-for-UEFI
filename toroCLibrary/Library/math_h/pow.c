/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    pow.c

Abstract:

    Implementation of the Standard C function.
    Calculates x raised to the power of y.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <math.h>

extern double __cdecl __cde80387FYL2X(double x, double y);
extern double modf(double, double*);
extern double __cde80387F2XM1(double);
extern double fabs(double x);

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
double pow(double x, double y)
{
    double product = -NAN;                                  // assume failure
    double absx = fabs(x);
    double intgr, fract;
    double signedOne = 0 == signbit(x) ? 1.0 : -1.0;
    double yxldx = __cde80387FYL2X(absx, y);                // product of y and ld(x)
    double twoPowerFract = 0.0;
    double twoPowerIntgr = 0.0;
    CDEDOUBLE* ptwoPowerIntgr = (void*) &twoPowerIntgr;
    double tmp;
    char fYIsFrational = 0.0 != modf(y, &tmp);
    char fYIsEven = 0 == fmod(y, 2.0);

    do 
    {
        if (fYIsFrational && (- 1.0 == signedOne))
            break;

        if(fYIsEven && -1.0 == signedOne)
			signedOne = 1.0;

        if(0.0 == fabs(y))
		{
			product = 1.0;
			break;
		}

        if (0.0 == absx) 
        {
            product = -signedOne * x;
            break;
        }

        fract = modf(yxldx, &intgr);

        //if (intgr > -1023)
        //    ptwoPowerIntgr->member.exp = 1023/*bias*/ + (uint64_t)intgr,
        //    twoPowerFract = 1.0 + __cde80387F2XM1(fract);

        twoPowerFract = 1.0 + __cde80387F2XM1(fract);

        if (intgr > -1023)  // deal with normalized doubles
            ptwoPowerIntgr->member.exp = 1023/*bias*/ + (uint64_t)intgr;
        else        // deal with subnormals
        {
            // -1023 -> (1 << 51)
            // -1024 -> (1 << 50)
            // ...
            // -1074 -> (1 <<  0)
            if (((int64_t)intgr + 1023 + 51) >= 0)
                ptwoPowerIntgr->member.mant = 1ULL << ((int64_t)intgr + 1023 + 51);
        }

        product = signedOne * twoPowerIntgr * twoPowerFract;

    }while(0);

    return product;
}
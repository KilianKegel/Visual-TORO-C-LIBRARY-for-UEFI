/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    modf.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Splits a floating-point value into fractional and integer parts.

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

extern double __cdecl __cde80387FISTP(double x);
extern uint16_t __cdecl __cde80387FSTCW(void);
extern void __cdecl __cde80387FLDCW(uint16_t);

/**

Synopsis
    #include <math.h>
    double modf(double x, double* intptr);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/modf-modff-modfl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/modf-modff-modfl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/modf-modff-modfl#return-value

**/
static double modfCDEABI(double x, double* intptr)
{
    double dRet = x;
    CDEDOUBLE* pdRet = (void*)&dRet;

    do {

        if (INFINITY == x || -INFINITY == x)
        {
            *intptr = x;

            dRet = x;
            pdRet->uint64 &= 0x8000000000000000ULL;//keep sign

            break;
        }


        *intptr = x;
        double d = x;
        uint64_t* pd = (void*)&dRet;
        uint64_t* pint64 = (void*)&d, fractmask;
        unsigned mask = ((1LL << (CDEDOUBLE_SIGPOS - CDEDOUBLE_EXPPOS)) - 1);
        unsigned bias = (1LL << (CDEDOUBLE_SIGPOS - CDEDOUBLE_EXPPOS - 1)) - 1; //1023
        signed exp = (*pint64 >> CDEDOUBLE_EXPPOS);
        char minus = 0 != ((1LL << CDEDOUBLE_SIGPOS) & *pint64);
        signed shift = 0;

        exp &= mask;
        exp = exp - bias;

        exp = exp > CDEDOUBLE_EXPPOS ? CDEDOUBLE_EXPPOS : exp;

        if (exp >= 0)
        {
            shift = (CDEDOUBLE_EXPPOS - exp);
            shift = shift >= 0 ? shift : 0;
            fractmask = ~((1LL << shift) - 1);
            *pint64 &= fractmask;
            *intptr = d;

            dRet = x - d;
            if (minus)
                *pd |= (1LL << CDEDOUBLE_SIGPOS);
        }
        else
            if (minus)
                *intptr = -0.0;
            else
                *intptr = 0.0;
    } while (0);
    return dRet;
}

MKCDEABI(modf);
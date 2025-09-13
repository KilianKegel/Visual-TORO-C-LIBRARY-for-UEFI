/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    sqrt.c

Abstract:

    Implementation of the Standard C function.
    Calculates the square root of a floating-point value.

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

extern double __cdecl __cde80387FSQRT(double x);

/**

Synopsis
    #include <math.h>
    double modf(double x, double* intptr);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl#return-value

**/
static double __cdecl sqrtCDEABI(double x)
{
    CDEDOUBLE d = { .dbl = x };
    CDEDOUBLE dRet;

    do
    {
        if (0x7FF == d.member.exp)
        {
            if (0ULL == d.member.mant) 
            {
                if(0 == d.member.sign)
                    dRet.uint64 = 0x7FF0000000000000ULL;
                else
                    dRet.uint64 = 0xFFF8000000000000ULL;
            }
            else
                dRet.uint64 = 0x0008000000000000ULL | d.uint64;
            break;
        }

        if (+0.0 <= x)
            dRet.dbl = __cde80387FSQRT(x);
        else
            dRet.uint64 = 0xFFF8000000000000ULL;

    } while (0);

    if (0xFFF8000000000000ULL == dRet.uint64)
        if (0xFFF8000000000000ULL != d.uint64)
            errno = EDOM;

    return dRet.dbl;
}

MKCDEABI(sqrt);
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    cos.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the cosine of a floating-point value.

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

extern double __cdecl __cde80387FCOS(double x);

/**

Synopsis
    #include <math.h>
    double cos(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cos-cosf-cosl
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cos-cosf-cosl#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/cos-cosf-cosl#return-value

**/
static double __cdecl cosCDEABI(double x)
{
    CDEDOUBLE d = { .dbl = x };
    CDEDOUBLE dRet = { .uint64 = 0x7FF8042000000000LL };
    
    do 
    {

        if (d.member.exp == 0x7FF)
        {
            dRet.uint64 = 0x0008000000000000ULL | d.uint64;

            if (0ULL == d.member.mant)
                dRet.uint64 = 0x8008000000000000ULL | d.uint64,
                errno = EDOM;

            break;
        }

        if ((63 + 1023) > d.member.exp)
            dRet.dbl = __cde80387FCOS(x);
        else
            errno = ERANGE;

    } while (0);

    return dRet.dbl;
}

MKCDEABI(cos);
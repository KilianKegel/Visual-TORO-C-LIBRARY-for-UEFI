/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    frexp.c

Abstract:

    Implementation of the Standard C function.
    Gets the mantissa and exponent of a floating-point number.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <errno.h>

extern double modf(double x, double* intptr);
extern double fabs(double x);

/**

Synopsis
    #include <math.h>
    double frexp(double x, int *expptr);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/frexp
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/frexp#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/frexp#return-value

**/
double __cdecl frexp(double x, int* expptr)
{
    CDEDOUBLE* px = (void*)&x;
    CDEDOUBLE dRet = { .dbl = x };
    CDEDOUBLE dbl = { .dbl = 0.0 };
    int exp = (int)px->member.exp - 1023;

    do {
        if (0.0 == x || -0.0 == x)
        {
            *expptr = 0;
            break;
        }

        if (0 != px->member.exp)                                // deal with normalized doubles
        {
            dbl.member.exp = (exp == 1024 ? -1 : 1 + px->member.exp);

            *expptr = exp + 1 > 1024 ? -1 : exp + 1;

            if (0x7FF == dbl.member.exp)
                dbl.member.exp = 0x7FE,
                dRet.dbl = x / dbl.dbl,
                dRet.member.exp &= (-1 == *expptr ? ~0 : ~1);
            else
                dRet.dbl = x / dbl.dbl;
        }
        else                                                    // deal with subnormals
        {
            for (int i = 51; i >= 0; i--)
            {
                if (px->member.mant & (1ULL << i))
                {
                    *expptr = -1073 + 0 + i;
                    dbl.uint64 = 1ULL << (1 + i);
                    break;
                }
            }
            dRet.dbl = x / dbl.dbl;
        }

    } while (0);

    //
    // errno
    //
    if (0x7FFULL == dRet.member.exp)
        if(dRet.uint64 != px->uint64)
            if ((0x0008000000000000ULL | px->uint64) == (dRet.uint64))
                errno = EDOM;

    return dRet.dbl;
}
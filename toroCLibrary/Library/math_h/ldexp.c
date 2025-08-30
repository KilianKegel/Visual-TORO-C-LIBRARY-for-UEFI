/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
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
#include <CdeServices.h>
#include <cde.h>
#include <errno.h>

extern double __cde80387FSCALE(double x, double y);

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
    CDEDOUBLE dRet = {.dbl = x};
    
    do 
    {
        if (0x7FFULL == dRet.member.exp)
        {
            if (0ULL != dRet.member.mant)
            {
                if (0ULL == ((1ULL << 51) & dRet.member.mant))  // if highest mantisse bit NOT set
                    errno = EDOM;                               // set domain error for NAN
                dRet.member.mant |= (1ULL << 51);               // set highest mantisse bit
            }
            break;                                              // return NAN
        }

        dRet.dbl = __cde80387FSCALE(x, (double)exp);

        //
        // check result
        //
        if(0x7FFULL == dRet.member.exp)
            errno = ERANGE;                                     // set range error for INF
    } while (0);
    
    return dRet.dbl;
}

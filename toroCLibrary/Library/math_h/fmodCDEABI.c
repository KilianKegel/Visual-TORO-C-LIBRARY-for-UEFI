/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    fmod.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the floating-point remainder.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <math.h>
//
// ERRNO.H
//
#define EDOM    33
#define ERANGE  34
#define errno   (*_errno())
extern __declspec(dllimport) int* _errno(void);

//extern double modf(double x, double* intptr);
extern double __cdecl __cde80387FPREM(double x, double y);

/**

Synopsis
    #include <math.h>
    double fmod(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fmod-fmodf
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fmod-fmodf#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fmod-fmodf#return-value

**/
static double __cdecl fmodCDEABI(double x, double y)
{
    CDEDOUBLE dRet, * py = (void*)&y , * px = (void*)&x;

    dRet.dbl = x;

    do
    {
        if (0.0 == y || -0.0 == y)
        {
            if (0x7FF == dRet.member.exp) {
                if (0 == dRet.member.mant)
                    dRet.member.mant |= 0x0008000000000000ULL;
                break;
            }
        }

        if (0x7FF == py->member.exp && 0Ull != py->member.mant) {
            dRet.dbl = y;
            break;
        }

        if (INFINITY == x || -INFINITY == x) {
            dRet.member.exp = 0x7FF;
            dRet.member.mant = 0x0008000000000000;
            break;
        }

        if (0.0 == y || -0.0 == y) {
            dRet.member.exp = 0x7FF;
            dRet.member.mant = 0x0008000000000000;
            dRet.uint64 = 0x7FF8000000000000ULL;
            break;
        }

        if (0x7FF == dRet.member.exp && 0ULL != dRet.member.mant)
            break;

        dRet.dbl = __cde80387FPREM(x, y);

    } while (0);

    //
    // errno
    //
    if (0x7FF8000000000000ULL == dRet.uint64 || 0xFFF8000000000000ULL == dRet.uint64)
        if (0x7FF8000000000000ULL != px->uint64
            && 0x7FF8000000000000ULL != py->uint64
            && 0xFFF8000000000000ULL != px->uint64
            && 0xFFF8000000000000ULL != py->uint64)
            errno = EDOM;

    return dRet.dbl;
}

MKCDEABI(fmod);
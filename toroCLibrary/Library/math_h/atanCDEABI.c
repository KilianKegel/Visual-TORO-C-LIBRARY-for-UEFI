/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    atan.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Calculates the arcatangent of a floating-point value.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern double __cdecl __cde80387FPATAN(double x);

/**

Synopsis
    #include <math.h>
    double atan(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l#return-value

**/
static double __cdecl atanCDEABI(double x)
{
    return __cde80387FPATAN(x);
}

MKCDEABI(atan);
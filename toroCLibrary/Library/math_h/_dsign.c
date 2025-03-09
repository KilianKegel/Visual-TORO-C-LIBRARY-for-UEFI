/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    tan.c

Abstract:

    Implementation of the floating-point primitive signbit macro or function in the CRT.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <math.h>
    int _dsign(double x);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floating-point-primitives#_dsign-_ldsign-_fdsign
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floating-point-primitives#_dsign-_ldsign-_fdsign#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floating-point-primitives#_dsign-_ldsign-_fdsign#return-value

**/
int _dsign(double d)
{
    CDEDOUBLE* pdbl = (void*)&d;

    return (int)pdbl->member.sign * 32768;
}
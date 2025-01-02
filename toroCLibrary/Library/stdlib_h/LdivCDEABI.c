/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    LdivCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Computes the quotient and the remainder of two integer values.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdlib.h
//
typedef struct _ldiv_t
{
    long quot;
    long rem;
} ldiv_t;

/**

Synopsis
    #include <stdlib.h>
    ldiv_t ldiv(long number, long denom);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/div?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/div?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/div?view=msvc-160#return-value
**/
static ldiv_t ldivCDEABI(long number, long denom) {
    ldiv_t r;

    r.quot = number / denom;
    r.rem = number % denom;
    return r;
}

MKCDEABI(ldiv);
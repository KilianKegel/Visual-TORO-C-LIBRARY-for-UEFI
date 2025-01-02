/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    srand.c

Abstract:

    Implementation of the Standard C function.
    Sets the starting seed value for the pseudorandom number generator used by the rand function.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <stdlib.h>
    void srand(unsigned int seed);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/srand?view=msvc-160&viewFallbackFrom=vs-2019
    The srand function uses the argument as a seed for a new sequence of pseudo-random
    numbers to be returned by subsequent calls to rand. If srand is then called with the
    same seed value, the sequence of pseudo-random numbers shall be repeated. If rand is
    called before any calls to srand have been made, the same sequence shall be generated
    as when srand is first called with a seed value of 1.
    The implementation shall behave as if no library function calls the srand function.
Returns
    The srand function returns no value.

    @param[in] seed

    @retval void

**/
void  srand(unsigned dwSeed) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    unsigned* pnext = (unsigned*)&pCdeAppIf->nNextRandStaticInternal;

    *pnext = dwSeed;
}
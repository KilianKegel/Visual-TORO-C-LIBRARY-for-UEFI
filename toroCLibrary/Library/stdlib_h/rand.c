/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    rand.c

Abstract:

    Implementation of the Standard C function.
    Generates a pseudorandom number.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdlib.h>

/**

Synopsis
    #include <stdlib.h>
    int rand(void);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand?view=msvc-160&viewFallbackFrom=vs-2019
    The rand function computes a sequence of pseudo-random integers in the range 0 to
    RAND_MAX.
    The implementation shall behave as if no library function calls the rand function.
Returns
    The rand function returns a pseudo-random integer.
    Environmental limits
    The value of the RAND_MAX macro shall be at least 32767.

    @param[in] void

    @retval randomized int

**/
int rand(void) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    unsigned* pnext = (unsigned*)&pCdeAppIf->nNextRandStaticInternal;

    return RAND_MAX & ((*pnext = *pnext * 214013 + 2531011) >> 16);
}

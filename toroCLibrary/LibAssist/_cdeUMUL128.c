/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeUMUL128.c

Abstract:

    CDE replacement for Microsoft _umul128() intrinsic, that works for 32 and 64 Bit platforms

    Microsoft Specific

    Multiplies two 64-bit unsigned integers passed in as the first two arguments and
    puts the high 64 bits of the product in the 64-bit unsigned integer pointed to by
    HighProduct and returns the low 64 bits of the product.
    https://docs.microsoft.com/en-us/cpp/intrinsics/umul128?view=msvc-170

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>

/** umul128()

Synopsis

    uint64_t _umul128(uint64_t Multiplier, uint64_t Multiplicand, uint64_t *HighProduct);

Description

    split up a string in a argc/argv equivalent.

Returns

    @param[in]  Multiplier
    @param[in]  Multiplicand
    @param[out] *HighProduct


    @retval The low 64 bits of the product.

**/
uint64_t _cdeUMUL128(uint64_t a, uint64_t b, uint64_t* pHigh)
{
    uint32_t res128[4] = { 0,0,0,0 };

    uint32_t* pA32 = (uint32_t*)&a, * pB32 = (uint32_t*)&b;
    uint64_t A64, B64, * pC64, * pRet = (uint64_t*)&res128[0], * pRetHi = (uint64_t*)&res128[2];

    for (int bx = 0; bx < 2; bx++)
        for (int ax = 0; ax < 2; ax++)
        {
            pC64 = (uint64_t*)&res128[ax + bx];  // index 0,1,1,2
            A64 = (uint64_t)pA32[ax];
            B64 = (uint64_t)pB32[bx];
            *pC64 += A64 * B64;
        }
    *pHigh = *pRetHi;
    return *pRet;
}

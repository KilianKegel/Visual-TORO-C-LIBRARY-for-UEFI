/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeMemStrxCpy.c

Abstract:

    CDE internal worker function for all mem/str/wcs-copy functions

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

void* _cdeMemStrxCpy(IN int preset, OUT void* pszDest, IN const void* pszSource, IN  size_t count) //,IN unsigned char fBreakOnZero/* distinguish mem- vs. strcpy */,IN unsigned char fWide)
{
    unsigned n;
    signed xcrement = PRESET_TDN ? -1 : 1;//increment vs decrement
    unsigned short  c;
    unsigned char* pDst8 = pszDest;
    unsigned short* pDst16 = pszDest;
    const unsigned char* pSrc8 = pszSource;
    const unsigned short* pSrc16 = pszSource;

    for (n = 0; n < count; n++, pDst8 = &pDst8[xcrement], pSrc8 = &pSrc8[xcrement], pDst16 = &pDst16[xcrement], pSrc16 = &pSrc16[xcrement])
    {

        if (PRESET_WID) {
            *pDst16 = *pSrc16;
            c = *pSrc16;
        }
        else {
            *pDst8 = *pSrc8;
            c = (unsigned short)*pSrc8;
        }

        if ((PRESET_BOZ) && c == '\0')
            break;
    }
    //
    // strncpy support -- that fills remaining buffer until count
    //
    if (((size_t) -1 != count) && PRESET_BOZ) {
        for ( /*n = 0*/; n < count; n++, pDst16 = &pDst16[xcrement], pDst8 = &pDst8[xcrement]) {

            if (PRESET_WID) {
                *pDst16 = '\0';
            }
            else {
                *pDst8 = '\0';
            }
        }
    }

    return pszDest;
}
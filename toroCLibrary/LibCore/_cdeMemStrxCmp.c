/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeMemStrxCmp.c

Abstract:

    CDE internal worker function for all mem/str/wcs-compare functions

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern short ydes(unsigned char fDes, signed i, short v);

signed _cdeMemStrxCmp(IN ROMPARM_MEMSTRXCMP* pFixParm, IN const char* pszDest, IN const char* pszSource, IN size_t count) {
    unsigned char fDes = 2 == (pFixParm->fForceToDataSeg & 2);
    register signed n = 0;
    wchar_t c1, c2, c1bORVal, c2bORVal;
    size_t  wCnt = (size_t)-1;
    unsigned char bORVal = ((!pFixParm->fCaseSensitive) << 5);  //ORVal is ORed to
                                                        //to ASCII A..Z, a..z
                                                        //it is 0x20 fCaseSensitive
                                                        // 0x00 otherwise
                                                        // patent pendig

    pszDest = pFixParm->fWide ? (/*const*/ char*)&(((wchar_t*)pszDest)[-1]) : &pszDest[-1];
    pszSource = pFixParm->fWide ? (/*const*/ char*)&(((wchar_t*)pszSource)[-1]) : &pszSource[-1];

    do {

        if (++wCnt == count)         // TODO: test 0 count
            break;                  // TODO: test 0 count

        pszDest = pFixParm->fWide ? (/*const*/ char*)&(((wchar_t*)pszDest)[1]) : &pszDest[1];
        pszSource = pFixParm->fWide ? (/*const*/ char*)&(((wchar_t*)pszSource)[1]) : &pszSource[1];

        c1 = pFixParm->fWide ? *((wchar_t*)pszDest) : *pszDest;
        c2 = pFixParm->fWide ? *((wchar_t*)pszSource) : *pszSource;
        c2 = ydes(fDes, (signed)wCnt, c2);

        c1bORVal = c1 + bORVal;
        c2bORVal = c2 + bORVal;

        c1 = ((c1 <= 'Z') && (c1 >= 'A')) ? c1bORVal : c1;
        c2 = ((c2 <= 'Z') && (c2 >= 'A')) ? c2bORVal : c2;

        n = c1 - c2;


        c1 = pFixParm->fBreakOnZero ? c1 : -1;

    } while (n == 0 && c1 != 0);

    if (pFixParm->fAjustDifference) {
        n = n < 0 ? -1 : (n > 0 ? 1 : n);     /*MS compatibility: return -1,0,1 only on non-"n" types*/
    }
    return n;
}

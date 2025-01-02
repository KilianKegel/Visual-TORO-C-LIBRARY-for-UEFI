/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeChkIsWxTbl.c

Abstract:

    Function checks, if a wide character belongs to a particular ISWX_TRUTH_TABLE

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <wctype.h>
#include <_wctype.h>

/**

Synopsis
    #include <CdeServices.h>
    char _cdeChkIsWxTbl(ISWX_TRUTH_TABLE *pIswxTbl, wint_t c);
Description

    Function checks the ISWX_TRUTH_TABLE

    @param[in] ISWX_TRUTH_TABLE *pIswxTbl
    @param[in] wint_t c

    @retval 0 - not present
            1 - present
**/
char _cdeChkIsWxTbl(ISWX_TRUTH_TABLE* pIswxTbl, wint_t c) {
    char nRet = 0;

    pIswxTbl--;
    do {
        pIswxTbl++;
        if (pIswxTbl->word0 < pIswxTbl->word1)
            nRet = (c >= pIswxTbl->word0 && c <= pIswxTbl->word1);
        else
            if (pIswxTbl->word0 > pIswxTbl->word1)
                nRet = (c == pIswxTbl->word0 || c == pIswxTbl->word1);
            else
                if (pIswxTbl->word0 == pIswxTbl->word1 && pIswxTbl->word0 != 0xFFFF)
                    nRet = (c == pIswxTbl->word0);

    } while (nRet == 0 && pIswxTbl->word0 != pIswxTbl->word1);

    return nRet;
}

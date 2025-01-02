/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iswdigitCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#define _CTYPE_DISABLE_MACROS
#include <_wctypeCDEABI.h>
#include <CdeServices.h>
//
// wctype.h
//
// Bit masks for the possible character types
#define _DIGIT   0x04     // digit[0-9]

extern char _cdeChkIsWxTbl(ISWX_TRUTH_TABLE* pIswxTbl, wint_t c);

extern ISWX_TRUTH_TABLE __cdeIswdigit_dflt_tbl[];
static ISWX_TRUTH_TABLE* gpTBLiswdigit = __cdeIswdigit_dflt_tbl;

/** Brief description of the function’s purpose.

Synopsis
    #include <wctype.h>
    int iswdigit(wint_t wc);
Description
    The iswdigit function tests for any wide character that corresponds to a decimal-digit
    character (as defined in 5.2.1).

    @param[in] c character to test for DIGIT

    @retval ISDIGIT if upcase letter

    @retval 0 if not

**/
static int iswdigitCDEABI(wint_t c) {

    return _DIGIT * _cdeChkIsWxTbl(gpTBLiswdigit, c);
}

MKCDEABI(iswdigit);
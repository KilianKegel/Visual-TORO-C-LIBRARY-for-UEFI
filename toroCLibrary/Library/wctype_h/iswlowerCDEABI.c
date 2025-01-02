/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iswlowerCDEABI.c

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
#define _LOWER   0x02     // lowercase letter


extern char _cdeChkIsWxTbl(ISWX_TRUTH_TABLE* pIswxTbl, wint_t c);

extern ISWX_TRUTH_TABLE __cdeIswlower_dflt_tbl[];
static ISWX_TRUTH_TABLE* gpTBLiswlower = __cdeIswlower_dflt_tbl;

/** Brief description of the function’s purpose.

Synopsis
    #include <wctype.h>
    int iswlower(wint_t wc);
Description
    The iswlower function tests for any wide character that corresponds to a lowercase
    letter or is one of a locale-specific set of wide characters for which none of iswcntrl,
    iswdigit, iswpunct, or iswspace is true.

    @param[in] c character to test for lowcase

    @retval ISLOWER if upcase letter

    @retval 0 if not

**/
static int iswlowerCDEABI(wint_t c) {

    return _LOWER * _cdeChkIsWxTbl(gpTBLiswlower, c);
}

MKCDEABI(iswlower);
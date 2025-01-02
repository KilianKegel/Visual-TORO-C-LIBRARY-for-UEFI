/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iswpunctCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#define _CTYPE_DISABLE_MACROS
#include <_wctype.h>
#include <CdeServices.h>
//
// wctype.h
//
typedef unsigned short                wint_t;
#define _PUNCT   0x10

extern char _cdeChkIsWxTbl(ISWX_TRUTH_TABLE* pIswxTbl, wint_t c);

extern ISWX_TRUTH_TABLE __cdeIswpunct_dflt_tbl[];
static ISWX_TRUTH_TABLE* gpTBLiswpunct = __cdeIswpunct_dflt_tbl;

/** Brief description of the function’s purpose.

Synopsis
    #include <wctype.h>
    int iswpunct(wint_t wc);
Description
    The iswpunct function tests for any printing wide character that is one of a localespecific
    set of punctuation wide characters for which neither iswspace nor iswalnum
    is true.

@param[in] c character to test for lowcase

@retval ISLOWER if upcase letter

@retval 0 if not

**/
static int iswpunctCDEABI(wint_t c) {

    return _PUNCT * _cdeChkIsWxTbl(gpTBLiswpunct, c);
}

MKCDEABI(iswpunct);
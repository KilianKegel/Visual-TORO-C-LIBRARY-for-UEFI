/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iswupperCDEABI.c

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
// wctype.
//
typedef unsigned short                wint_t;
#define _UPPER   0x01

extern char _cdeChkIsWxTbl(ISWX_TRUTH_TABLE* pIswxTbl, wint_t c);

extern ISWX_TRUTH_TABLE __cdeIswupper_dflt_tbl[];
static ISWX_TRUTH_TABLE* gpTBLiswupper = __cdeIswupper_dflt_tbl;

/** Brief description of the function’s purpose.

Synopsis
    #include <wctype.h>
    int iswupper(wint_t wc);
Description
    The iswupper function tests for any wide character that corresponds to an uppercase
    letter or is one of a locale-specific set of wide characters for which none of iswcntrl,
    iswdigit, iswpunct, or iswspace is true

@param[in] c character to test

@retval _UPPER if upcase letter

@retval 0 if not

**/
static int iswupperCDEABI(wint_t c) {

    return _UPPER * _cdeChkIsWxTbl(gpTBLiswupper, c);
}

MKCDEABI(iswupper);
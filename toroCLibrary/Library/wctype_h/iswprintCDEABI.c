/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iswprintCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#define _CTYPE_DISABLE_MACROS
#include <CdeServices.h>
//
// wchar.h
//
typedef unsigned short                wint_t;

extern __declspec(dllimport) int iswgraph(wint_t c);
extern __declspec(dllimport) int iswblank(wint_t c);

/** Brief description of the function’s purpose.

Synopsis
    #include <wctype.h>
    int iswprint(wint_t wc);
Description
    The iswprint function tests for any printing wide character.

    @param[in] c character to test for isprint

    @retval _ALPHA | ISDIGIT | ISPUNCT if matching letter

    @retval 0 if not

**/
static int iswprintCDEABI(wint_t c) {

    int nRet0 = 0, nRet1 = 0;

    if (c != '\t') {
        nRet0 = iswgraph(c);

        if (0 == nRet0)
            nRet1 = iswblank(c);

    }
    return nRet0 | nRet1;

}

MKCDEABI(iswprint);
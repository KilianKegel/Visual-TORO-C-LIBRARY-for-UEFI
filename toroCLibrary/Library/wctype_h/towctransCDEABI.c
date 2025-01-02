/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    towctransCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#include <_wctypeCDEABI.h>
#include <CdeServices.h>
//
// wctype.
//
typedef wchar_t wctrans_t;
#define _UPPER 0x01

extern __declspec(dllimport) wint_t towupper(wint_t c);
extern __declspec(dllimport) wint_t towlower(wint_t c);

/** Brief description of the function�s purpose.

Synopsis
    #include <wctype.h>
    wint_t towctrans(wint_t wc, wctrans_t desc);
Description
    The towctrans function maps the wide character wc using the mapping described by
    desc. The current setting of the LC_CTYPE category shall be the same as during the call
    to wctrans that returned the value desc.
    Each of the following expressions behaves the same as the call to the wide character case
    mapping function (7.25.3.1) in the comment that follows the expression:

        towctrans(wc, wctrans("tolower")) // towlower(wc)
        towctrans(wc, wctrans("toupper")) // towupper(wc)

Returns
    The towctrans function returns the mapped value of wc using the mapping described
    by desc.

@param[in] c character to test

@retval _UPPER if upcase letter

@retval 0 if not

**/
static wint_t towctransCDEABI(wint_t c, wctrans_t value) {

    return value == _UPPER ? towupper(c) : towlower(c);
}

MKCDEABI(towctrans);
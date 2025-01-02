/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wctransCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#include <_wctypeCDEABI.h>
#include <CdeServices.h>
//
// wchar.h
//
typedef wchar_t wctrans_t;
#define _UPPER 0x01
#define _LOWER 0x02

extern __declspec(dllimport) int strcmp(const char* pszDst, const char* pszSrc);

/** Brief description of the function�s purpose.

Synopsis
    #include <wctype.h>
    wctrans_t wctrans(const char *property);
Description
    The wctrans function constructs a value with type wctrans_t that describes a
    mapping between wide characters identified by the string argument property.
    The strings listed in the description of the towctrans function shall be valid in all
    locales as property arguments to the wctrans function.
Returns
    Ifproperty identifies a valid mapping of wide characters according to the LC_CTYPE
    category of the current locale, the wctrans function returns a nonzero value that is valid
    as the second argument to the towctrans function; otherwise, it returns zero.

@param[in] c character to test for lowcase

@retval _UPPER on "toupper" / _LOWER on "tolower"

@retval 0 otherwise

**/
static wctrans_t wctransCDEABI(const char* name) {

    wctrans_t nRet = 0;

    if (0 == strcmp(name, "toupper"))
        nRet = _UPPER;
    else
        if (0 == strcmp(name, "tolower"))
            nRet = _LOWER;

    return nRet;
}

MKCDEABI(wctrans);
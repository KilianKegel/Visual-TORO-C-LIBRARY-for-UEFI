/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wctypeCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#include <_wctypeCDEABI.h>
#include <CdeServices.h>

#define ELC(x)/*element count*/ (sizeof(x) / sizeof(x[0]))
extern __declspec(dllimport) int strcmp(const char* pszDst, const char* pszSrc);
extern WCPROPERTYCDEABI __cdeWcpropertyCDEABI[];

/** Brief description of the function�s purpose.

Synopsis
    #include <wctype.h>
    wctype_t wctype(const char *property);
Description
    The wctype function constructs a value with type wctype_t that describes a class of
    wide characters identified by the string argument property.

    The strings listed in the description of the iswctype function shall be valid in all
    locales as property arguments to the wctype function.
Returns
    If property identifies a valid class of wide characters according to the LC_CTYPE
    category of the current locale, the wctype function returns a nonzero value that is valid
    as the second argument to the iswctype function; otherwise, it returns zero.

@param[in] const char *property -> "alpha","alnum","blank","cntrl","digit","graph","lower","print","punct","space","upper","xdigit"

@retval non zero on success

@retval 0 if not

**/
static wint_t wctypeCDEABI(const char* property) {
    wint_t nRet = 0;
    int i = -1;

    if (NULL != property) {
        while (NULL != __cdeWcpropertyCDEABI[++i].szProperty) {
            if (0 == strcmp(property, __cdeWcpropertyCDEABI[i].szProperty)) {
                nRet = __cdeWcpropertyCDEABI[i].type;
                break;
            }
        }
    }

    return nRet;
}

MKCDEABI(wctype);
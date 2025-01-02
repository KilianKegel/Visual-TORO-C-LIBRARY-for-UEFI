/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    localeconvCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Gets detailed information on locale settings.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern struct _CDE_LCONV_LANGUAGE* _locales[];
//
// _locale.h
//
extern __declspec(dllimport) void* memcpy(void* s1, const void* s2, size_t n);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char     int_frac_digits;
    char     frac_digits;
    char     p_cs_precedes;
    char     p_sep_by_space;
    char     n_cs_precedes;
    char     n_sep_by_space;
    char     p_sign_posn;
    char     n_sign_posn;
    wchar_t* _W_decimal_point;
    wchar_t* _W_thousands_sep;
    wchar_t* _W_int_curr_symbol;
    wchar_t* _W_currency_symbol;
    wchar_t* _W_mon_decimal_point;
    wchar_t* _W_mon_thousands_sep;
    wchar_t* _W_positive_sign;
    wchar_t* _W_negative_sign;
};

/** localeconv()
*
Synopsis

    #include <locale.h>
    struct lconv *localeconv( void );

Description

    Gets detailed information on locale settings.
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/localeconv?view=msvc-160&viewFallbackFrom=vs-2019

Paramters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/localeconv?view=msvc-160#return-value

**/
static struct lconv* localeconvCDEABI(void) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    if (NULL == pCdeAppIf->ActiveLocale.pCopy) {

        pCdeAppIf->ActiveLocale.pCopy = pCdeAppIf->pCdeServices->pMemRealloc(
            pCdeAppIf,
            NULL,
            sizeof(struct lconv),
            &pCdeAppIf->pCdeServices->HeapStart
        );
        //
        //NOTE: This is MSFT implementation, copy once. GNU copies the structure to the same buffer
        //      each time.
        memcpy(pCdeAppIf->ActiveLocale.pCopy, pCdeAppIf->ActiveLocale.pLconv, sizeof(struct lconv));
    }
    return pCdeAppIf->ActiveLocale.pCopy;
}
MKCDEABI(localeconv);
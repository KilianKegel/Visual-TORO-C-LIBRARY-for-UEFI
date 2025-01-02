/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    setlocale.c

Abstract:

    Implementation of the Standard C function.
    Sets or retrieves the run-time locale.
    NOTE:   minimized functionality
            only LC_ALL and "C" accepted as parameters

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <string.h>
#include <locale.h>
extern struct _CDE_LCONV_LANGUAGE* _locales[];

/** setlocale()
*
Synopsis

    #include <locale.h>
    char *setlocale(int category, const char *locale);

Description

    Sets or retrieves the run-time locale.
    NOTE:   minimized functionality
            only LC_ALL and "C" accepted as parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-160&viewFallbackFrom=vs-2019

Paramters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-160#return-value

**/
char* setlocale(int nCat, char const* szLocale) {

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char* pszRet = NULL;
    struct _CDE_LCONV_LANGUAGE** ppLocales = &_locales[-1];

    do {
        if (nCat != LC_ALL)
            break;
        
        if (NULL == szLocale)
            break;

        while ((*++ppLocales)->szLanguageCountry != NULL) {

            if (0 == strcmp((*ppLocales)->szLanguageCountry, szLocale))
            {
                memcpy(&pCdeAppIf->ActiveLocale,ppLocales,sizeof(struct _CDE_LCONV_LANGUAGE));
                pszRet = (*ppLocales)->szLanguageCountry;
                break;
            }
        }

    } while (0);
    return pszRet;

}
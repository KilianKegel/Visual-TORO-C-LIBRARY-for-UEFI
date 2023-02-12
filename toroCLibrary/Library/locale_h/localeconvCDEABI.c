/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
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
#include <_locale.h>

extern struct _CDE_LCONV_LANGUAGE* _locales[];
//extern __declspec(dllimport) void* memcpy(void* s1, const void* s2, size_t n);

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

    if (NULL == (*pCdeAppIf).pActiveLocale->pCopy) {

        (*pCdeAppIf).pActiveLocale->pCopy = pCdeAppIf->pCdeServices->pMemRealloc(
            pCdeAppIf,
            NULL,
            sizeof(struct lconv),
            &pCdeAppIf->pCdeServices->HeapStart
        );
        //
        //NOTE: This is MSFT implementation, copy once. GNU copies the structure to the same buffer
        //      each time.
        memcpy((*pCdeAppIf).pActiveLocale->pCopy, (*pCdeAppIf).pActiveLocale->pLconv, sizeof(struct lconv));
    }


    return (*pCdeAppIf).pActiveLocale->pCopy;
}

MKCDEABI(localeconv);
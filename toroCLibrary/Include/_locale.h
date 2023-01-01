/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _locale.h

Abstract:

    Definition of CDE specific structure _CDE_LCONV_LANGUAGE to deal with ANSI C LOCALE.H

Author:

    Kilian Kegel

--*/
#ifndef __LOCALE_H_
#define __LOCALE_H_
#include <wchar.h>
#include <locale.h>

struct _CDE_LCONV_LANGUAGE
{
    char *szLanguageCountry;
    struct lconv* pLconv;
    struct lconv* pCopy;    // copy returned for localeconv()
};

#endif//__LOCALE_H_

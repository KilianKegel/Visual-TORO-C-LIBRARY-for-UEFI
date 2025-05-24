/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _locales.c

Abstract:

    Implementation of only available "C" locale for "toro C Library"

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <locale.h>

extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;

static struct _CDE_LCONV_LANGUAGE EndOfTable;/* = { NULL,NULL };*/

struct _CDE_LCONV_LANGUAGE* _locales[] = {

    &_cdeCLocale ,
    &EndOfTable

};
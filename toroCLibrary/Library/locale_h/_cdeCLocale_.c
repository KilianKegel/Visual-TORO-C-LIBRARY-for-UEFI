/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCLocale.c

Abstract:

    Implementation of only available "C" locale for "toro C Library"

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <locale.h>
#include <limits.h>

struct lconv _cdeLconv_C = {
    .decimal_point = ".",
    .thousands_sep = "",
    .grouping = "",
    .int_curr_symbol = "",
    .currency_symbol = "",
    .mon_decimal_point = "",
    .mon_thousands_sep = "",
    .mon_grouping = "",
    .positive_sign = "",
    .negative_sign = "",
    .int_frac_digits = CHAR_MAX,
    .frac_digits = CHAR_MAX,
    .p_cs_precedes = CHAR_MAX,
    .p_sep_by_space = CHAR_MAX,
    .n_cs_precedes = CHAR_MAX,
    .n_sep_by_space = CHAR_MAX,
    .p_sign_posn = CHAR_MAX,
    .n_sign_posn = CHAR_MAX,
    ._W_decimal_point = L".",
    ._W_thousands_sep = L"",
    ._W_int_curr_symbol = L"",
    ._W_currency_symbol = L"",
    ._W_mon_decimal_point = L"",
    ._W_mon_thousands_sep = L"",
    ._W_positive_sign = L"",
    ._W_negative_sign = L"",
};

struct _CDE_LCONV_LANGUAGE _cdeCLocale = { "C", &_cdeLconv_C, NULL };

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _iswblank_dflt_tbl.c

Abstract:

    wide blank ISWX TRUTH table

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <wctype.h>
#include <_wctype.h>

ISWX_TRUTH_TABLE __cdeIswblank_dflt_tbl[] = { \
    {0x0020, 0x0009},\
    {0xFEFF, 0x3000},\
    {0x3000, 0x3000},\
    { 0xFFFF, 0xFFFF } \
};//iswBLANK_dflt_tbl[]
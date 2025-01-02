/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _iswspace_dflt_tbl.c

Abstract:

    wide space ISWX TRUTH table

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <wctype.h>
#include <_wctype.h>

ISWX_TRUTH_TABLE __cdeIswspace_dflt_tbl[] = { \
{0x0009, 0x000D}, \
{0x2000, 0x200A}, \
{0x2028, 0x2029}, \
{0x0085, 0x0020}, \
{0x1680, 0x00A0}, \
{0x202F, 0x180E}, \
{0x3000, 0x205F}, \
{0xFFFF, 0xFFFF} \
};//iswSPACE_dflt_tbl[]

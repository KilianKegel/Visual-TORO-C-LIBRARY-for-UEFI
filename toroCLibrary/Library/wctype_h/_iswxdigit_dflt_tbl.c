/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _iswxdigit_dflt_tbl.c

Abstract:

    wide xdigit ISWX TRUTH table

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <wctype.h>
#include <_wctype.h>

ISWX_TRUTH_TABLE __cdeIswxdigit_dflt_tbl[] = { \
{0x0030, 0x0039}, \
{0x0041, 0x0046}, \
{0x0061, 0x0066}, \
{0xFF10, 0xFF19}, \
{0xFF21, 0xFF26}, \
{0xFF41, 0xFF46}, \
{0xFFFF, 0xFFFF} \
};//iswXDIGIT_dflt_tbl[]

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _iswcntrl_dflt_tbl.c

Abstract:

    wide control ISWX TRUTH table

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#include <wctype.h>
#include <_wctype.h>

ISWX_TRUTH_TABLE __cdeIswcntrl_dflt_tbl[] = { \
    {0x0000, 0x001F}, \
    {0x007F, 0x009F}, \
    {0x0600, 0x0603}, \
    {0x17B4, 0x17B5}, \
    {0x200B, 0x200F}, \
    {0x202A, 0x202E}, \
    {0x2060, 0x2064}, \
    {0x206A, 0x206F}, \
    {0xFFF9, 0xFFFB}, \
    {0x06DD, 0x00AD}, \
    {0xFEFF, 0x070F}, \
    {0xFFFF, 0xFFFF} \
};//iswCNTRL_dflt_tbl[]
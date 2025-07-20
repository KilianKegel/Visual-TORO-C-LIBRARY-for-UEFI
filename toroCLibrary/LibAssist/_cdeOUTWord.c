/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeOUTWord.c

Abstract:

    Replacement of Microsoft specific intrinsic function __outword().

Author:

    Kilian Kegel

--*/
#include <intrin.h>
void _cdeOUTWord(unsigned short p, unsigned short v)
{
    __outword(p, v);
}
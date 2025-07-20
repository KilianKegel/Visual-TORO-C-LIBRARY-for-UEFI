/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeINWord.c

Abstract:

    Replacement of Microsoft specific intrinsic function _cdeINWord().

Author:

    Kilian Kegel

--*/
#include <intrin.h>
unsigned short _cdeINWord(unsigned short p)
{
    return __inword(p);
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeINDWord.c

Abstract:

    Replacement of Microsoft specific intrinsic function __indword().

Author:

    Kilian Kegel

--*/
#include <intrin.h>
unsigned long _cdeINDWord(unsigned short p)
{
    return __indword(p);
}
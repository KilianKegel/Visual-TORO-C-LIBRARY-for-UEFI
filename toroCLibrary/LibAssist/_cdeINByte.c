/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeINByte.c

Abstract:

    Replacement of Microsoft specific intrinsic function _cdeINByte().

Author:

    Kilian Kegel

--*/
#include <intrin.h>
unsigned char _cdeINByte(unsigned short p)
{
    return __inbyte(p);
}
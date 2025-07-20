/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __inword.c

Abstract:

    CLANG/LLVM specific intrinsic function.

Author:

    Kilian Kegel

--*/
#ifdef __clang__
unsigned short __inword(unsigned short p)
{
    unsigned short v;
    __asm__ __volatile__("inw %w[p], %w[v]": [v] "=a" (v) : [p] "Nd" (p));
    return v;
}
#endif// __clang__


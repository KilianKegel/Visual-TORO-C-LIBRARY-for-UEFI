/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __indword.c

Abstract:

    CLANG/LLVM specific intrinsic function.

Author:

    Kilian Kegel

--*/
#ifdef __clang__
unsigned int __indword(unsigned short p)
{
    unsigned int v;
    __asm__ __volatile__("inl %w[p], %k[v]": [v] "=a" (v) : [p] "Nd" (p));
    return v;
}
#endif// __clang__


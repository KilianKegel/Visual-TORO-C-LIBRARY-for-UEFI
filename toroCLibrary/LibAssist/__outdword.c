/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __outdword.c

Abstract:

    CLANG/LLVM specific intrinsic function.

Author:

    Kilian Kegel

--*/
#ifdef __clang__
void __outdword(unsigned short p, unsigned long v)
{
    __asm__ __volatile__("outl %k[v], %w[p]"::[p] "Nd" (p), [v] "a" (v));
}
#endif// __clang__
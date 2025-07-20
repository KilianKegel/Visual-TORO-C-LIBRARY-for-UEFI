/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _intel_fast_memset.c

Abstract:

    CLANG/LLVM specific intrinsic function.

Author:

    Kilian Kegel

--*/
#ifdef __clang__
#include <stddef.h>
void* _intel_fast_memset(void* s, int c, size_t n) {
    volatile unsigned char* p;
    size_t i;

    for (i = 0, p = (unsigned char*)s; i < n; i++)
        p[i] = (unsigned char)c;
    return s;
}
#endif//__clang__
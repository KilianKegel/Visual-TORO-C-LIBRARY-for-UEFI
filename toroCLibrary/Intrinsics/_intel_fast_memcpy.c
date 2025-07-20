/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _intel_fast_memcpy.c

Abstract:

    CLANG/LLVM specific intrinsic function.

Author:

    Kilian Kegel

--*/
#ifdef __clang__
#include <stddef.h>
void* _intel_fast_memcpy(void* s1, const void* s2, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        ((unsigned char*)s1)[i] = ((unsigned char*)s2)[i];
    }
    return s1;
}
#endif//__clang__
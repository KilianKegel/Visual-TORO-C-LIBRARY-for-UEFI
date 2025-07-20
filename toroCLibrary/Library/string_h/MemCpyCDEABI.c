/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    MemCpyCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Copies bytes between buffers.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
/**
Synopsis
    #include <string.h>
    void* memcpy(void* s1, const void* s2, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memcpy-wmemcpy?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memcpy-wmemcpy?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memcpy-wmemcpy?view=msvc-160#return-value
**/
static void* memcpyCDEABI(void* s1, const void* s2, size_t n) {
    size_t i;

    volatile unsigned char* dst = (unsigned char*)s1;
    volatile unsigned char* src = (unsigned char*)s2;

    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return s1;
}

MKCDEABI(memcpy);
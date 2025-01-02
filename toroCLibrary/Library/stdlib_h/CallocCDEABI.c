/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CallocCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Allocates an array in memory with elements initialized to 0.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) void* realloc(void* ptr, size_t size);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);

/**

Synopsis
    #include <stdlib.h>
    void *calloc(size_t nmemb, size_t size);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/calloc?view=msvc-160&viewFallbackFrom=vs-2019
    The calloc function allocates space for an array of nmemb
    objects, each of whose size is size. The space is initialized to all bits zero.
Returns
    The calloc function returns either a null pointer or a pointer to the allocated space.
**/

static void* callocCDEABI(size_t nmemb, size_t size) {
    void* pRet;
    pRet = realloc(0, nmemb * size);
    if (pRet) {
        memset(pRet, 0, nmemb * size);
    }
    return pRet;
}

MKCDEABI(calloc);
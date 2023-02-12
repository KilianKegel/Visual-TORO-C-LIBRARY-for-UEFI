/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FreeCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Deallocates or frees a memory block.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) void* realloc(void* ptr, size_t size);

/**

Synopsis
    #include <stdlib.h>
    void free(void* ptr);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/free?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/free?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/free?view=msvc-160#remarks
**/
static void freeCDEABI(void* ptr) {
    realloc(ptr, 0);
}

MKCDEABI(free);

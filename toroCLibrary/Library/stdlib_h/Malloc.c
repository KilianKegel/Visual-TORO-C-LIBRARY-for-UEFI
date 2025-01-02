/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Malloc.c

Abstract:

    Implementation of the Standard C function.
    Allocates memory blocks.

Author:

    Kilian Kegel

--*/
#include <stdlib.h>
/**

Synopsis
    #include <stdlib.h>
    void* malloc(size_t size);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=msvc-160#return-value
**/
void* malloc(size_t size) {
    return realloc(0, size);
}

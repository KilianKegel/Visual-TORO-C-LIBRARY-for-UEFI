/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Free.c

Abstract:

    Implementation of the Standard C function.
    Deallocates or frees a memory block.

Author:

    Kilian Kegel

--*/
#include <stdlib.h>

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
void free(void* ptr) {

    if(NULL != ptr)
        realloc(ptr, 0);
}

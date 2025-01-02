/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ReallocCORE.c

Abstract:

    Implementation of the Standard C function.
    Reallocates a memory block.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <stdlib.h>
    void* __cdereallocCORE(CDE_APP_IF *pCdeAppIf, void* ptr, size_t size);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-160#return-value

**/
void* __cdereallocCORE(CDE_APP_IF* pCdeAppIf, void* ptr, size_t size) {

    void* pRet;

    pRet = pCdeAppIf->pCdeServices->pMemRealloc(
        pCdeAppIf,
        ptr,    //IN void *ptr,   /* input pointer for realloc */
        size,   //IN size_t size, /*  input size for realloc*/
        &pCdeAppIf->pCdeServices->HeapStart
    );
    return pRet;
}


/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    MemCmpCDEINTRINABI.c

Abstract:

    Monolithic COMDAT __declspec(dllimport) and __cdecl Library version

    Implementation of the Standard C function.
    Compares characters in two buffers.

Author:

    Kilian Kegel

--*/
#include <stddef.h>
#include <CdeServices.h>

static ROMPARM_MEMSTRXCMP ROMPARM = {
    /*fForceToDataSeg*/ 1 ,\
    /*fCountIsParm*/    1,  \
    /*fCaseSensitive*/  1,  \
    /*fBreakOnZero*/    0,  \
    /*fAjustDifference*/1,  \
    /*fWide*/           0 };
/**
Synopsis
    #include <string.h>
    int memcmp(const void *buffer1, const void *buffer2, size_t count);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memcmp-wmemcmp?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memcmp-wmemcmp?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memcmp-wmemcmp?view=msvc-160#return-value
**/
int memcmp(const void* pszDst, const void* pszSrc, size_t count) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return pCdeAppIf->pCdeServices->pMemStrxCmp(&ROMPARM, pszDst, pszSrc, count);

}

#if   defined(_M_AMD64)
void* __imp_memcmp = (void*)memcmp;/* CDE MAKE DLL IMPORT */
#else//   defined(_M_AMD64)
void* _imp__memcmp = (void*)memcmp;/* CDE MAKE DLL IMPORT */
#endif//  defined(_M_AMD64)

char __cdeMemCmpCDEINTRINABIAnchor;
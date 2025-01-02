/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTMemAlloc.c

Abstract:

    OS interface (osif) memory allocation for Windows NT

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <CdeServices.h>
#define UINT64 unsigned long long 

//
// prototype
//
OSIFMEMALLOC _osifWinNTMemAlloc;

/** _osifMemAlloc() - allocate memory  pages

Synopsis
    #include <CdeServices.h>
    HEAPDESC* _osifWinNTMemAlloc(IN void*pCdeAppIf, IN unsigned Pages);
Description
    Get memory pages.
Parameters
    CDE_APP_IF* pCdeAppIf   - application interface
    unsigned int Pages      - number of pages
Returns
    pointer to memory block
**/
HEAPDESC* _osifWinNTMemAlloc(IN CDE_APP_IF* pCdeAppIf, IN size_t Pages) {

    HEAPDESC* pMemory = 0;

    pMemory = (HEAPDESC*)VirtualAlloc(NULL, (size_t)(4096 * Pages), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (pMemory)
        pMemory->fInalterable = FALSE/*PEIIF == pCdeAppIf->DriverParm.CommParm.OSIF*/;

    return pMemory;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTMemAlloc.c

Abstract:

    OS interface (osif) memory allocation for Windows NT

Author:

    Kilian Kegel

--*/
#include <windows.h>

#define UINT64 unsigned long long 

typedef struct _HEAPDESC {
    UINT64  qwMagic;
#define FREEMEM 0x4D454552463E3E3EL     /* >>>FREEM */
#define ALLOCMEM 0x434F4C4C413E3E3EL    /* >>>ALLOC */
#define ENDOFMEM 0x464F444E453E3E3EL    /* >>>ENDOF */
    unsigned char fInalterable;
    struct _HEAPDESC* pPred;
    struct _HEAPDESC* pSucc;
    UINT64/*EFI_PHYSICAL_ADDRESS*/ PageBase;
    unsigned  Pages;
}HEAPDESC, * PHEAPDESC;

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
HEAPDESC* _osifWinNTMemAlloc(IN void/*CDE_APP_IF*/* pCdeAppIf, IN unsigned Pages) {

    HEAPDESC* pMemory = 0;

    pMemory = (HEAPDESC*)VirtualAlloc(NULL, (size_t)(4096 * Pages), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (pMemory)
        pMemory->fInalterable = FALSE/*PEIIF == pCdeAppIf->DriverParm.CommParm.OSIF*/;

    return pMemory;
}
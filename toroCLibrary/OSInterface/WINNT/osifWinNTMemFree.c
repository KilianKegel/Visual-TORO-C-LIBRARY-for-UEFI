/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTMemFree.c

Abstract:

    OS interface (osif) memory free for Windows NT

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <CdeServices.h>

typedef unsigned long long EFI_PHYSICAL_ADDRESS;

//
// prototype
//
OSIFMEMFREE _osifWinNTMemFree;

/** _osifMemFree() - free memory  pages

Synopsis
    #include <CdeServices.h>
    void _osifWinNTMemFree(IN void* pCdeAppIf, IN EFI_PHYSICAL_ADDRESS Memory, IN unsigned Pages);
Description
    PEI can not release pages
Parameters
    CDE_APP_IF* pCdeAppIf       - application interface
    EFI_PHYSICAL_ADDRESS Memory - 64Bit physical address
    unsigned int Pages          - number of pages
Returns
    void
**/
void _osifWinNTMemFree(IN CDE_APP_IF* pCdeAppIf, IN EFI_PHYSICAL_ADDRESS Memory, IN size_t Pages) {

    void* p;

    p = &Memory;

    VirtualFree(p, 0, MEM_RELEASE);

    return;//PEI can not release pages
}
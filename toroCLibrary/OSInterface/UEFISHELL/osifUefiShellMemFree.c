/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellMemFree.c

Abstract:

    OS interface (osif) memory free for UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <PiPei.h>

#include <Base.h>
#include <CdeServices.h>

//
// prototype
//
OSIFMEMFREE _osifUefiShellMemFree;

/** _osifMemFree() - free memory  pages

Synopsis
    #include <CdeServices.h>
    void _osifUefiShellMemFree(IN CDE_APP_IF* pCdeAppIf, IN EFI_PHYSICAL_ADDRESS Memory, IN unsigned Pages);
Description
    PEI can not release pages
Parameters
    CDE_APP_IF* pCdeAppIf       - application interface
    EFI_PHYSICAL_ADDRESS Memory - 64Bit physical address
    unsigned int Pages          - number of pages
Returns
    void
**/
void _osifUefiShellMemFree(IN CDE_APP_IF* pCdeAppIf, IN EFI_PHYSICAL_ADDRESS Memory, IN  size_t Pages) {
    EFI_STATUS Status;
    Status = pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->BootServices->FreePages(Memory, Pages);
    
    return;
}
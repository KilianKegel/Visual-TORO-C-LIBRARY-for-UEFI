/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiDxeMemAlloc.c

Abstract:

    OS interface (osif) memory allocation for UEFI DXE

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>

//
// prototype
//
OSIFMEMALLOC _osifUefiDxeMemAlloc;

/** _osifMemAlloc() - allocate memory  pages

Synopsis
    #include <CdeServices.h>
    HEAPDESC* _osifUefiDxeMemAlloc(IN CDE_APP_IF* pCdeAppIf, IN unsigned int Pages);
Description
    Get memory pages.
Parameters
    CDE_APP_IF* pCdeAppIf   - application interface
    unsigned int Pages      - number of pages
Returns
    pointer to memory block
**/
HEAPDESC* _osifUefiDxeMemAlloc(IN CDE_APP_IF* pCdeAppIf, IN size_t Pages) {

    EFI_PHYSICAL_ADDRESS Memory = (EFI_PHYSICAL_ADDRESS)-1; //NOTE: iapx86 specific -> little endian dependant
    HEAPDESC** ppMemory = (HEAPDESC**)&Memory;              //NOTE: iapx86 specific -> little endian dependant
    EFI_STATUS Status;
    
    Status = pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->BootServices->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        Pages,
        &Memory);

    if (Status == EFI_SUCCESS) {
        (*ppMemory)->fInalterable = FALSE/*PEIIF == pCdeAppIf->DriverParm.CommParm.OSIf*/;
    }
    else
        Memory = 0LL;

    return *ppMemory;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiPeiEntryPoint.c

Abstract:

    CRT0 for PEI in a non-EDK build environment

Author:

    Kilian Kegel

--*/
//
// block the original tianocore/UEFI DriverEntryPoint.c
//

#define _DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_   // block _cdeCRT0UefiDxeEDK() from beeing included
#define _DEBUG_LIB_C_H_                             // block DebugLib.c from beeing included
#define _PEIM_ENTRY_POINT_C_H_                      // block DriverEntryPoint.c from beeing included
#define _CDE_STDC_INTRINSICS_C_H_                   // provide __cdecl memset(), __cdecl memcmp() in the EntryPPoint .OBJ

#include "osifUefiPeiEntryPointEDK.c"

/** _cdeCRT0UefiPei() - CRT0 for PEI in a non-EDK build environment

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiPei(IN EFI_PEI_FILE_HANDLE FileHandle, IN const EFI_PEI_SERVICES** PeiServices);
Description
    C runtime initialization.
Parameters
    EFI_PEI_FILE_HANDLE FileHandle
    const EFI_PEI_SERVICES** PeiServices
Returns
    Status
**/
EFI_STATUS EFIAPI _cdeCRT0UefiPei/*for non-EDK build environment*/(IN EFI_PEI_FILE_HANDLE FileHandle, IN const EFI_PEI_SERVICES** PeiServices)
{
    return _MainEntryPointPei(FileHandle, PeiServices);
}
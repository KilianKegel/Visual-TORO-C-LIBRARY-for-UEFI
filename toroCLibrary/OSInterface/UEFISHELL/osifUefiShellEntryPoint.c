/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiShellEntryPoint.c

Abstract:

    CRT0 for UEFI Shell in a non-EDK build environment

Author:

    Kilian Kegel

--*/
//
// block the original tianocore/UEFI DriverEntryPoint.c
//
#define _DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_   // block _cdeCRT0UefiDxeEDK() from beeing included
#define _DEBUG_LIB_C_H_                             // block DebugLib.c from beeing included
#define _APPLICATION_ENTRY_POINT_C_H_               // block DriverEntryPoint.c from beeing included
#define _CDE_STDC_INTRINSICS_C_H_                   // provide __cdecl memset(), __cdecl memcmp() in the EntryPPoint .OBJ

#include "osifUefiShellEntryPointEDK.c"

EFI_HANDLE               _cdegImageHandle;
EFI_SYSTEM_TABLE*        _cdegST;
EFI_BOOT_SERVICES*       _cdegBS;
EFI_RUNTIME_SERVICES*    _cdegRT;

/** _cdeCRT0UefiShell() - CRT0 for UEFI Shell in a non-EDK build environment

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiDxe(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
Description
    C runtime initialization.
Parameters
    IN EFI_HANDLE        ImageHandle
    IN EFI_SYSTEM_TABLE* SystemTable
Returns
    Status
**/
EFI_STATUS EFIAPI _cdeCRT0UefiShell/*for non-EDK build environment*/(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    _cdegImageHandle = ImageHandle;
    _cdegST = SystemTable;
    _cdegBS = SystemTable->BootServices;
    _cdegRT = SystemTable->RuntimeServices;
//
    return _MainEntryPointShell(ImageHandle, SystemTable);
}


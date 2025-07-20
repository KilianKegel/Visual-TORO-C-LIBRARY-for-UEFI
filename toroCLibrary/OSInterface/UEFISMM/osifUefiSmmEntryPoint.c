/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiSmmEntryPoint.c

Abstract:

    CRT0 for UEFI SMM in a non-EDK build environment

Author:

    Kilian Kegel

--*/
//
// block the original tianocore/UEFI DriverEntryPoint.c
//
#define _DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_   // block _cdeCRT0UefiDxeEDK() from beeing included
#define _DEBUG_LIB_C_H_             // block DebugLib.c from beeing included
#define _DRIVER_ENTRY_POINT_C_H_    // block DriverEntryPoint.c from beeing included
#define _CDE_STDC_INTRINSICS_C_H_                   // provide __cdecl memset(), __cdecl memcmp() in the EntryPPoint .OBJ

//
// include the entire .C file but with _cdeCRT0UefiDxeEDK() and _cdeCRT0UefiDxeEDKINT3() disabled by switches above
//
#include "osifUefiSmmEntryPointEDK.c"

EFI_HANDLE _cdegImageHandle;
EFI_SYSTEM_TABLE* _cdegST;
EFI_BOOT_SERVICES* _cdegBS;
EFI_RUNTIME_SERVICES* _cdegRT;
EFI_SMM_SYSTEM_TABLE2* _cdegSmst;

/** _cdeCRT0UefiDxe() - CRT0 for DXE in a non-EDK build environment

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiSmm(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
Description
    C runtime initialization.
Parameters
    IN EFI_HANDLE        ImageHandle
    IN EFI_SYSTEM_TABLE* SystemTable
Returns
    Status
**/
EFI_STATUS EFIAPI _cdeCRT0UefiSmm/*for non-EDK build environment*/(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    //
    // initialize EDK global variables USUALLY provided by UefiBootServicesTableLib.lib and UefiRuntimeServicesTableLib.lib
    //

    EFI_STATUS Status = EFI_ABORTED;
    
    _cdegImageHandle = ImageHandle;
    _cdegST = SystemTable;
    _cdegBS = SystemTable->BootServices;
    _cdegRT = SystemTable->RuntimeServices;
    _cdegSmst = NULL;                           // SMM system table initialized below

    do {
        //
        // get the SMM System Table
        //
        if (1)
        {
            EFI_SMM_BASE2_PROTOCOL* pSmmBase2 = NULL;

            Status = _cdegBS->LocateProtocol(
                &_gEfiSmmBase2ProtocolGuid,
                NULL,
                (VOID**)&pSmmBase2
            );

            if (EFI_SUCCESS != Status)
                break;

            //
            // We are in SMM, retrieve the pointer to SMM System Table
            //
            pSmmBase2->GetSmstLocation(pSmmBase2, &_cdegSmst);

            if (_cdegSmst == NULL)
                break;

            if (pSmmBase2->InSmm)
                break;
        }

        //
        // get the CdeServices
        //
        Status = SystemTable->BootServices->LocateProtocol(&_gCdeSmmProtocolGuid, NULL, (void**)&CdeAppIfSmm.pCdeServices);
        if (EFI_SUCCESS != Status)
        {
            __cdeFatalCdeServicesNotAvailSmm(SystemTable);
            break;//do{}while(0);
        }
        //
        // set handle, SytemTable and SMM Systemtable zo CdeAppIf
        //
        CdeAppIfSmm.DriverParm.BsDriverParm.ImageHandle = ImageHandle;
        CdeAppIfSmm.DriverParm.BsDriverParm.pSystemTable = SystemTable;
        CdeAppIfSmm.DriverParm.BsDriverParm.pSmmSystemTable2 = _cdegSmst;

    } while (Status = EFI_SUCCESS, Status == EFI_SUCCESS);

    return EFI_SUCCESS == Status ? _MainEntryPointSmm(ImageHandle, SystemTable) : Status;
}
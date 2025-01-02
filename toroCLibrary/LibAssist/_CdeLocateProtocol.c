/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeLocateProtocol.c

Abstract:

    CDE internal locate protocol

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <CdeServices.h>

extern EFI_SYSTEM_TABLE* _cdegST;

/**

Synopsis

    void* _CdeLocateProtocol(IN EFI_GUID* Protocol, IN void* Registration OPTIONAL/*,OUT void **Interface* /)

Description

    LoacteProtocol as a simple function

Returns

    @param[in] EFI_GUID* Protocol
    @param[in] void* Registration OPTIONAL

    @retval argc

**/
void* _CdeLocateProtocol(IN EFI_GUID* Protocol, IN void* Registration OPTIONAL/*,OUT void **Interface*/) {
    
    void* pRet;
    EFI_STATUS Status;

    Status = _cdegST->BootServices->LocateProtocol(Protocol, NULL, &pRet);
    return Status == EFI_SUCCESS ? pRet : NULL;
}
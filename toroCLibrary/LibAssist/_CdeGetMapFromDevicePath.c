/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeGetMapFromDevicePath.c

Abstract:

    CDE internal get drive mapping from device path

Author:

    Kilian Kegel

--*/

#include <uefi.h>
#include <Protocol\Shell.h>
#include <CdeServices.h>
#include <stdio.h>

extern void* _cdePoolWcs2AppWcs(short* pwcs, unsigned char freePool);

/** _CdeGetMapFromDevicePath()

Synopsis

    short* _CdeGetMapFromDevicePath(IN CDE_APP_IF *pCdeAppIf, IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath)

Description

    get drive mapping from device path

Returns

    @param[in] EFI_DEVICE_PATH_PROTOCOL** DevicePath

    @retval short *

**/
wchar_t* _CdeGetMapFromDevicePath(IN CDE_APP_IF *pCdeAppIf, IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath) 
{
    EFI_SHELL_PROTOCOL* pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;

    return _cdePoolWcs2AppWcs((short*)pEfiShellProtocol->GetMapFromDevicePath(DevicePath), 0/*FREEPOOL*/);
}
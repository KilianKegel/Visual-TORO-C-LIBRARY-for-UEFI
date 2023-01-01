/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
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

extern void* _cdePoolWcs2AppWcs(short* pwcs, unsigned char freePool);
extern  EFI_SHELL_PROTOCOL* pEfiShellProtocol;

/** _CdeGetMapFromDevicePath()

Synopsis

    short* _CdeGetMapFromDevicePath(IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath)

Description

    get drive mapping from device path

Returns

    @param[in] EFI_DEVICE_PATH_PROTOCOL** DevicePath

    @retval short *

**/
short* _CdeGetMapFromDevicePath(IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath) 
{
    return _cdePoolWcs2AppWcs((short*)pEfiShellProtocol->GetMapFromDevicePath(DevicePath), 0/*FREEPOOL*/);
}
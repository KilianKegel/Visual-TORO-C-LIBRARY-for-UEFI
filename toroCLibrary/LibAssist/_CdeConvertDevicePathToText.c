/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeConvertDevicePathToText.c

Abstract:

    Convert device path to text.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#undef NULL
#include <uefi.h>
#include <Protocol\Shell.h>
#include <Protocol\DevicePathToText.h>

extern void* _cdePoolWcs2AppWcs(wchar_t* pwcs, unsigned char freePool);
extern EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* pCdeEfiDevicePathToTextProtocol;

/** _cdeConvertDevicePathToText()

Synopsis

    short* _cdeConvertDevicePathToText(IN const EFI_DEVICE_PATH_PROTOCOL* DevicePath, IN unsigned char DisplayOnly, IN unsigned char AllowShortcuts)

Description

    Convert device path to text.

Returns

    @param[in] EFI_DEVICE_PATH_PROTOCOL** DevicePath

    @retval short * pwcsDevPath

**/
short* _cdeConvertDevicePathToText(IN const EFI_DEVICE_PATH_PROTOCOL* DevicePath, IN unsigned char DisplayOnly, IN unsigned char AllowShortcuts) 
{
    return _cdePoolWcs2AppWcs(pCdeEfiDevicePathToTextProtocol->ConvertDevicePathToText(IN DevicePath, IN DisplayOnly, IN AllowShortcuts), 0/*FREEPOOL*/);
}
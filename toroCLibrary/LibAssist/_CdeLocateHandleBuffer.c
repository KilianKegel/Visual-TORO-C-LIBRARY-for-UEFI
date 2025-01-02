/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeLocateHandleBuffer.c

Abstract:

    CDE internal Locate handle buffer

Author:

    Kilian Kegel

--*/
#include <stddef.h>
#undef NULL
#include <uefi.h>
// 
// stdlib.h
//
extern __declspec(dllimport) void* malloc(size_t size);
//extern __declspec(dllimport) void free(void* ptr);
//
// string.h
//
extern __declspec(dllimport) void* memcpy(void* s1, const void* s2, size_t n);
#include <CdeServices.h>
#include <cde.h>

#include <Protocol\Shell.h>
#include <Protocol\DevicePathToText.h>

extern EFI_SYSTEM_TABLE* _cdegST;

/**

Synopsis

    EFI_STATUS _CdeLocateHandleBuffer(IN EFI_LOCATE_SEARCH_TYPE SearchType, IN EFI_GUID* Protocol OPTIONAL, IN void* SearchKey OPTIONAL, IN OUT UINTN* NoHandles, OUT EFI_HANDLE** Buffer CDE_OPTIONAL)

Description

    Locate handle buffer

Returns

    @param[in] EFI_DEVICE_PATH_PROTOCOL** DevicePath

    @retval EFI_STATUS
    @retval unsigned long long* NoHandles
    @retval EFI_HANDLE** Buffer CDE_OPTIONAL
**/
EFI_STATUS _CdeLocateHandleBuffer(
    IN EFI_LOCATE_SEARCH_TYPE SearchType,
    IN EFI_GUID* Protocol OPTIONAL,
    IN void* SearchKey OPTIONAL,
    IN OUT UINTN* NoHandles,
    OUT EFI_HANDLE** Buffer CDE_OPTIONAL) {

    EFI_STATUS Status;
    EFI_STATUS nRet;
    EFI_HANDLE* HandleBuffer;

    do {
        nRet = _cdegST->BootServices->LocateHandleBuffer(SearchType, Protocol, SearchKey, NoHandles, &HandleBuffer);

        if (EFI_SUCCESS != nRet)
            break;

        if (NULL != Buffer) { /*CDE_OPTIONAL*/
            *Buffer = malloc(sizeof(EFI_HANDLE) * *NoHandles);
            memcpy(*Buffer, HandleBuffer, sizeof(EFI_HANDLE) * *NoHandles);
        }

        Status = _cdegST->BootServices->FreePool(HandleBuffer);

    } while (0);

    return nRet;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeEfiStatus2int.c

Abstract:

    CDE internal EFI_STATUS value sizing to integer

Author:

    Kilian Kegel

--*/
#include <stdint.h>
#include <stddef.h>

size_t _cdeInt2EfiStatus(int intstatus)
{
    uintptr_t efistatus = (uintptr_t)-1;
    long long mask = 0x800000007FFFFFFFLL;

    switch (sizeof(uintptr_t))
    {
    case sizeof(uint32_t) :
        efistatus = intstatus;
        break;
    default:
        efistatus = (0x80000000 == (0xC0000000 & intstatus)) ? ((intptr_t)(mask)) & intstatus : intstatus;
    }

    return efistatus;
}

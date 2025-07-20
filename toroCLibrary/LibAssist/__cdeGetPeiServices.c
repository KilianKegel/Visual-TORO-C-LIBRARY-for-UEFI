/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeGetPeiServices.c

Abstract:

    Get PeiServices from the global storage location

Author:

    Kilian Kegel

--*/
#include <stdint.h>
#include <CdeServices.h>
#include <intrin.h>
/** __cdeGetPeiServices()

Synopsis

    void __cdeGetPeiServices(void);
Description

    Get PeiServices from the global storage location
    https://uefi.org/sites/default/files/resources/PI_Spec_1_7_A_final_May1.pdf#page=89

Returns

    void* PeiServices
**/
void* __cdeGetPeiServices(void)
{
    uint16_t buffer16[5];// = { 0x1111,0x2222, 0x3333, 0x4444, 0x5555 };
    void** ppIdtBase;

    __sidt(&buffer16[0]);

    ppIdtBase = *((void**)(&buffer16[1]));

    return ppIdtBase[-1];
}


/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CDE_DXE_PROTOCOL_GUID.c

Abstract:

    Definition of CDE_DXE_PROTOCOL_GUID

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <uefi.h>
#include <Cde.h>

EFI_GUID _gCdePeiProtocolGuid = CDE_PEI_PROTOCOL_GUID;          // The GUID for the protocol

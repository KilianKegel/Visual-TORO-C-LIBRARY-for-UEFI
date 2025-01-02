/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    EFI_STATUS_CODE_RUNTIME_PROTOCOL_GUID.c

Abstract:

    Definition of EFI_STATUS_CODE_RUNTIME_PROTOCOL_GUID

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <uefi.h>
#include <Pi\PiStatusCode.h>
#include <Protocol\ShellParameters.h>

EFI_GUID _gEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

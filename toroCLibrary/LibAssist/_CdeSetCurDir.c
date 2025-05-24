/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CDE internal change current directory

Abstract:

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <Protocol\Shell.h>
#include <CdeServices.h>

extern void* _cdePoolWcs2AppWcs(unsigned short* pwcs, unsigned char freePool);

/** _CdeSetCurDir()

Synopsis

    const short * _CdeSetCurDir(IN const short* FileSystemMapping)

Description

    change current directory

Returns

    @param[in] const short* FileSystemMapping

    @retval void *

**/
EFI_STATUS _CdeSetCurDir(IN CDE_APP_IF *pCdeAppIf, IN const unsigned short* FileSystem, IN const unsigned short* Dir) 
{
    EFI_SHELL_PROTOCOL* pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;

    return pEfiShellProtocol->SetCurDir(FileSystem, Dir);
}
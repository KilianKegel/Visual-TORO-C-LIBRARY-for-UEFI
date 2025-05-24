/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeGetCurDir.c

Abstract:

    CDE internal get current directory

Author:

    Kilian Kegel

--*/

#include <uefi.h>
#include <Protocol\Shell.h>
#include <CdeServices.h>

extern void* _cdePoolWcs2AppWcs(short* pwcs, unsigned char freePool);

/** _CdeGetCurDir() 

Synopsis

    const short * _CdeGetCurDir(IN CDE_APP_IF *pCdeAppIf, IN const short* FileSystemMapping)

Description

    get current directory

Returns

    @param[in] const short* FileSystemMapping

    @retval short * pwcsDirectoryName

**/
const short * _CdeGetCurDir(IN CDE_APP_IF *pCdeAppIf, IN const unsigned short* FileSystemMapping) 
{
    EFI_SHELL_PROTOCOL* pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;

    return _cdePoolWcs2AppWcs((short*)pEfiShellProtocol->GetCurDir(FileSystemMapping), 1/*FREEPOOL*/);
}

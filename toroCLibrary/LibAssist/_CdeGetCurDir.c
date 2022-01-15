/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
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

extern void* _cdePoolWcs2AppWcs(short* pwcs, unsigned char freePool);
extern  EFI_SHELL_PROTOCOL* pEfiShellProtocol;

/** _CdeGetCurDir() 

Synopsis

    const short * _CdeGetCurDir(IN const short* FileSystemMapping)

Description

    get current directory

Returns

    @param[in] const short* FileSystemMapping

    @retval short * pwcsDirectoryName

**/
const short * _CdeGetCurDir(IN const short* FileSystemMapping) 
{
    return _cdePoolWcs2AppWcs((short*)pEfiShellProtocol->GetCurDir(FileSystemMapping), 1/*FREEPOOL*/);
}

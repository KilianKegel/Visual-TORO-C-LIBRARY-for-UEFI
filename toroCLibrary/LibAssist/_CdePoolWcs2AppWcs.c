/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdePoolWcs2AppWcs.c

Abstract:

    Move wide character string from pool memory to malloced memory


Author:

    Kilian Kegel

--*/
#include <stddef.h>
#undef NULL
#include <uefi.h>
//
// wchar.h
//
extern __declspec(dllimport) wchar_t* wcscpy(wchar_t* pszDst, const wchar_t* pszSrc);
extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);

//
// stdlib.h
//
extern __declspec(dllimport) void* malloc(size_t size);

#include <CdeServices.h>

extern EFI_SYSTEM_TABLE* _cdegST;
extern BOOLEAN _gCdeCfgCmdLnParmPoolMemRetain; //ASSERT c:\svn_code\edk2_code\MdePkg\Library\UefiMemoryAllocationLib\MemoryAllocationLib.c(819): !EFI_ERROR (Status)

/** _cdePoolWcs2AppWcs()

Synopsis

    void* _cdePoolWcs2AppWcs(short* pwcs, unsigned char freePool)

Description

    Allocate local memory via malloc() that is freed automatically after process termination
    and replaces internally pool allocated memory that needs to be freed manually

Returns

    @param[in] short* pwcs
    @param[in] unsigned char freePool

    @retval void *

**/
void* _cdePoolWcs2AppWcs(unsigned short* pwcs, unsigned char freePool) {
    EFI_STATUS Status;
    unsigned short* pRet = NULL;
    size_t wlen;

    if (NULL != pwcs) {
        wlen = wcslen(pwcs);
        pRet = malloc((1 + wlen) * sizeof(wchar_t));
        wcscpy(pRet, pwcs);
        if (FALSE != freePool && _gCdeCfgCmdLnParmPoolMemRetain == 0) {
            Status = _cdegST->BootServices->FreePool(pwcs);
        }
    }

    return (void*)pRet;
}
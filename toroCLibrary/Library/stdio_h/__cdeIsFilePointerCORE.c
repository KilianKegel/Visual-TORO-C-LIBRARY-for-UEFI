/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeIsFilePointerCORE.c

Abstract:

    Toro C Library internal helperfunction that validates, if a FILE* pointer
    belongs to the internal I/O buffer

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <errno.h>
#include <CdeServices.h>

extern int __cdeOnErrSet_errnoCORE(CDE_APP_IF *pCdeAppIf, CDE_STATUS Status, int Error);
extern void* __cdeGetIOBufferCORE(CDE_APP_IF *pCdeAppIf, unsigned i);

/**

Synopsis

    char __cdeIsFilePointerCORE(CDE_APP_IF *pCdeAppIf, void* stream);

Description
    
    Toro C Library internal helperfunction that validates, if a FILE* pointer 
    belongs to the internal I/O buffer AND is allocated.

    It set errno to EBADF on fail.

Parameters

    void * stream   :   FILE* to check

Returns

    (1 + fd)    on SUCCESS fdplusone
    0           on FAILURE

**/
int __cdeIsFilePointerCORE(CDE_APP_IF *pCdeAppIf, void* stream) {
    int j;
    void* pCdeFile;
    EFI_STATUS Status = (EFI_STATUS)-1;

    for (j = 0, pCdeFile = __cdeGetIOBufferCORE(pCdeAppIf, 0)/*&_iob[0]*/; NULL != pCdeFile; pCdeFile = __cdeGetIOBufferCORE(pCdeAppIf, ++j))
    {
        if (pCdeFile == stream && TRUE == ((CDEFILE*)pCdeFile)->fRsv) {
            Status = EFI_SUCCESS;
            break;
        }
    }

    __cdeOnErrSet_errnoCORE(pCdeAppIf, Status, EBADF);

    return EFI_SUCCESS == Status ? 1 + j : 0;
}

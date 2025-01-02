/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeIsFilePointer.c

Abstract:

    Toro C Library internal helperfunction that validates, if a FILE* pointer
    belongs to the internal I/O buffer

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <errno.h>
#include <CdeServices.h>

extern int __cdeOnErrSet_errno(CDE_STATUS Status, int Error);

/**

Synopsis

    char __cdeIsFilePointer(void* stream);

Description
    
    Toro C Library internal helperfunction that validates, if a FILE* pointer 
    belongs to the internal I/O buffer. Allocation-state is not checked.

Parameters

    void * stream   :   FILE* to check

Returns

    (1 + fd)    on SUCCESS fdplusone
    0           on FAILURE

**/
int __cdeIsIOBuffer(void* stream) {
    int j;
    void* pCdeFile;
    EFI_STATUS Status = (EFI_STATUS)-1;

    for (j = 0, pCdeFile = __cdeGetIOBuffer(0)/*&_iob[0]*/; NULL != pCdeFile; pCdeFile = __cdeGetIOBuffer(++j))
    {
        if (pCdeFile == stream /* && TRUE == ((CDEFILE*)pCdeFile)->fRsv*/) {
            Status = EFI_SUCCESS;
            break;
        }
    }

    return EFI_SUCCESS == Status ? 1 + j : 0;
}
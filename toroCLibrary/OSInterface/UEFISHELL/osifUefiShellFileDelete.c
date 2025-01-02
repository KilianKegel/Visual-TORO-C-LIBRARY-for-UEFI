/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileDelete.c

Abstract:

    OS interface (osif) to delete a file UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <stdio.h>

#undef NULL
#include <CdeServices.h>

/**
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellFileDelete(CDE_APP_IF* pCdeAppIf, const char* filename, CDEFILE* pCdeFile);
Description
    Delete a file
Paramters
    CDE_APP_IF* pCdeAppIf       : unused
    const char* filename        : filename
    CDEFILE* pCdeFile           : CDEFILE* to file
Returns
    0   : success
    EOF : failure
**/
int _osifUefiShellFileDelete(CDE_APP_IF* pCdeAppIf, const char* filename, CDEFILE* pCdeFile)
{
    EFI_STATUS Status;

    do {

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Delete(pCdeFile->pFileProtocol));

    } while (0);

    return EFI_SUCCESS == Status ? 0 : EOF;
}
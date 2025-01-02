/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileClose.c

Abstract:

    OS interface (osif) close file for UEFI Shell

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
#define OS_EFI
#include <Base.h>
#include <CdeServices.h>
#include <stdio.h>

/**
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellFileClose(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile);
Description
    Close a file
Paramters
    CDE_APP_IF* pCdeAppIf       : unused
    CDEFILE* pCdeFile           : CDEFILE* to file
Returns
    0   : success
    EOF : failure
**/
int _osifUefiShellFileClose(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile)
{
    EFI_STATUS Status;

    do {

        if (O_RDONLY != (pCdeFile->openmode & (O_RDONLY | O_WRONLY | O_RDWR)))   // don't flush ro files
        {
            Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Flush(pCdeFile->pFileProtocol));

            if (Status != EFI_SUCCESS)break;
        }

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Close(pCdeFile->pFileProtocol));

        if (Status != EFI_SUCCESS)break;

    } while (0);

    return EFI_SUCCESS == Status ? 0 : EOF;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileSetPos.c

Abstract:

    OS interface (osif) set file read/write pointer UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <PiPei.h>
#include <Base.h>
#include <CdeServices.h>
#include <stdio.h>
#include <errno.h>
#include <cde.h>

/**
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, const fpos_t* pos)
Description
    Set file read/write pointer
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    CDEFILE* pCdeFile           : CDEFILE* file handle
    const fpos_t* pos           : new position + CDE_FPOS_SEEKEND marks a SEEK_END
Returns
    CDEFILE*: success
    NULL    : failure
**/
int _osifUefiShellFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, const fpos_t* pos)
{
    EFI_STATUS Status = EFI_SUCCESS;
    fpos_t eofpos, newpos = *pos;

    do {
        if (pCdeFile->openmode & O_CDENOSEEK/* if e.g. the file is a console, don't try to seek that will fail */)
            break;

        if (*pos & CDE_FPOS_SEEKEND) {     // SEEK_END flag set?

            Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->SetPosition(pCdeFile->pFileProtocol, 0xFFFFFFFFFFFFFFFFULL));

            //CDEFAULT( (CDEFINE_FATALSTATUS "%s\n",strefierror(efierrno)) );

            if (EFI_SUCCESS != Status) break;

            Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->GetPosition(pCdeFile->pFileProtocol, &eofpos));
            //CDEFAULT( (CDEFINE_FATALSTATUS "%s\n",strefierror(efierrno)) );

            if (EFI_SUCCESS != Status) break;

            newpos = eofpos + (~CDE_FPOS_SEEKEND & *pos);

        }

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->SetPosition(pCdeFile->pFileProtocol, newpos));
        pCdeFile->bpos = newpos;

    } while (0);
    return EFI_SUCCESS == Status ? 0 : EOF;
}
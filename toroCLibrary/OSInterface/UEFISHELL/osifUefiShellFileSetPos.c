/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileSetPos.c

Abstract:

    OS interface (osif) set file read/write pointer UEFI Shell

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
#define OS_EFI
#include <uefi.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

//#include <cde.h>
#include <CdeServices.h>

extern fpos_t __cdeOffsetCdeFposType(fpos_t fpos);
extern int __cdeBiasCdeFposType(fpos_t fpos);
OSIFFSETPOS _osifUefiShellFileSetPos;

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
int _osifUefiShellFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, CDEFPOS_T* pos)
{
    EFI_STATUS Status = EFI_SUCCESS;
    fpos_t eofpos = 0LL, newpos = __cdeOffsetCdeFposType(pos->fpos64);
    uint32_t dwMoveMethod = __cdeBiasCdeFposType(pos->fpos64);

    do {
        if (pCdeFile->openmode & O_CDENOSEEK    /* if e.g. the file is a console, don't try to seek that will fail */)
            break;

        if ((CDE_SEEK_BIAS_APPEND & CDE_SEEK_BIAS_MSK) == dwMoveMethod)
            newpos = 0LL,
            dwMoveMethod = SEEK_END;

        if (SEEK_END == dwMoveMethod) {     // SEEK_END flag set?

            Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->SetPosition(pCdeFile->pFileProtocol, 0xFFFFFFFFFFFFFFFFULL));

            if (EFI_SUCCESS != Status) break;

            Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->GetPosition(pCdeFile->pFileProtocol, (UINT64*)&eofpos));

            if (EFI_SUCCESS != Status) break;

            newpos += eofpos;
        }

        //
        // UEFI BUG: file positioning bug, if data written behind EOF, data range between old EOF and 
        // new data contains medium data / garbage, instead of 0
        //
        if (1/*KG20220418 gap of non-initialized disk space*/)
        {
            if (    EFI_SUCCESS == pCdeFile->pRootProtocol->SetPosition(pCdeFile->pFileProtocol, 0xFFFFFFFFFFFFFFFFULL)
                &&  EFI_SUCCESS == pCdeFile->pRootProtocol->GetPosition(pCdeFile->pFileProtocol, (UINT64*)&pCdeFile->gappos))
            {
                pCdeFile->gapsize = (newpos > pCdeFile->gappos) ? (size_t)(newpos - pCdeFile->gappos) : 0;
            }
            else {
            }

        }
        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->SetPosition(pCdeFile->pFileProtocol, newpos));
        pCdeFile->bpos = newpos;

    } while (0);
    return EFI_SUCCESS == Status ? 0 : EOF;
}
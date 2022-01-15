/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileSetPos.c

Abstract:

    OS interface (osif) set file read/write pointer UEFI Shell

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <windows.h>
#include <CdeServices.h>

#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void
#define CDE_FPOS_SEEKEND (1LL << 63)

/**
Synopsis
    #include <CdeServices.h>
    int _osifWinNTFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, const fpos_t* pos)
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
int _osifWinNTFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, const fpos_t* pos) {

    BOOL f = 1;
    BOOL seek_eof = 0 != (*pos & CDE_FPOS_SEEKEND);
    fpos_t fpostmp = *pos & ~CDE_FPOS_SEEKEND;
    LARGE_INTEGER* pPosLI = (LARGE_INTEGER*)&fpostmp;


    do {

        if (pCdeFile->openmode & O_CDENOSEEK/* if e.g. the file is a console, don't try to seek that will fail */)
            break;
        f = SetFilePointerEx(
            (HANDLE)pCdeFile->emufp,                            /*__in          HANDLE hFile,*/
            *pPosLI,                                            /*__in          LARGE_INTEGER liDistanceToMove,*/
            pPosLI,                                             /*__out_opt     PLARGE_INTEGER lpNewFilePointer,*/
            seek_eof ? FILE_END : FILE_BEGIN	/*SEEK_END*/    /*__in          DWORD dwMoveMethod*/
            );

    } while (0);

    pCdeFile->bpos = fpostmp;

    return f ? 0 : EOF;
}
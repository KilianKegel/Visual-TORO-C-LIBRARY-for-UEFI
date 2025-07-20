/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
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

extern fpos_t __cdeOffsetCdeFposType(fpos_t fpos);
extern int __cdeBiasCdeFposType(fpos_t fpos);
OSIFFSETPOS _osifWinNTFileSetPos;

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
int _osifWinNTFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, CDEFPOS_T* pos) {

    BOOL f = 1;
    LARGE_INTEGER ofs;
    uint32_t dwMoveMethod = __cdeBiasCdeFposType(pos->fpos64);

    ofs.QuadPart = __cdeOffsetCdeFposType(pos->fpos64);

    do {

        if (pCdeFile->openmode & O_CDENOSEEK/* if e.g. the file is a console, don't try to seek that will fail */)
            break;
        
        if ((CDE_SEEK_BIAS_APPEND & CDE_SEEK_BIAS_MSK) == dwMoveMethod)
            ofs.QuadPart = 0LL,
            dwMoveMethod = FILE_END/*SEEK_END*/;

        f = SetFilePointerEx(
            (HANDLE)pCdeFile->emufp,    /*__in          HANDLE hFile,*/
            ofs,                        /*__in          LARGE_INTEGER liDistanceToMove,*/
            &ofs,                       /*__out_opt     PLARGE_INTEGER lpNewFilePointer,*/
            dwMoveMethod                /*__in          DWORD dwMoveMethod*/
            );

    } while (0);

    pCdeFile->bpos = ofs.QuadPart;                                  

    return f ? 0 : EOF;
}
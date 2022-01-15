/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileClose.c

Abstract:

    OS interface (osif) close file for Windows NT


Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <windows.h>



#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void

typedef struct tagCDEFILE {
    unsigned char fRsv;                       // 0 if free, 1 if taken /reserved / occupied / allocated
    int     openmode;
    void* emufp;                        // true FILE fp used for emulation in Windows. In true EFI it is the CDEFILE fp
    //long   fpos;                      // fpos -> what ftell returns relative to SEEK_SET
    char* Buffer;                       // internal buffer
    long    bsiz;                       // sizeof internal buffer
    fpos_t  bpos;                       // buffers position equivalent to file pointer position. bpos[63] == SEEK_END marker CDE_FPOS_SEEKEND!!!
    long    bidx;                       // index into Buffer[] ranges from 0 .. bufsiz
    long    bvld;                       // number of valid bytes in the buffer beginning from
    unsigned char bdirty;               // buffer is dirty -> the buffer conatains unwritten characters
    unsigned char bclean;               // buffer is clean -> the buffer conatains characters read from the stream
        //NOTE: dirty and clean TRUE together, implements the microsoft strategy for common read/write data held in a buffer
        //      If CLEAN AND DIRTY are set simoultanously, file writes inhibited
    unsigned char fEof;                        // EOF flag for file
    unsigned char fErr;                        // ERR flag for file
    void* pRootProtocol;
    void* pFileProtocol;
    void* pwcsFileDrive;
    void* pwcsFilePath;
    void* pwcsFileName;
}CDEFILE;

/**
Synopsis
    #include <CdeServices.h>
    int _osifWinNTFileClose(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile);
Description
    Close a file
Paramters
    CDE_APP_IF* pCdeAppIf       : unused
    CDEFILE* pCdeFile           : CDEFILE* to file
Returns
    0   : success
    EOF : failure
**/
int _osifWinNTFileClose(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile)
{
    BOOL f;
    f = CloseHandle((HANDLE)pCdeFile->emufp);
    return 0;
}

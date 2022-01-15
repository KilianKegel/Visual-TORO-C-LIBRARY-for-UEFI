/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileWrite.c

Abstract:

    OS interface (osif) write file Windows NT

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <windows.h>

typedef unsigned long long EFI_PHYSICAL_ADDRESS;
typedef unsigned long long UINT64;
typedef void CDE_APP_IF;
typedef void EFI_FILE_PROTOCOL;

typedef struct tagCDEFILE {
    unsigned char fRsv;                       // 0 if free, 1 if taken /reserved / occupied / allocated
    int     openmode;
    void* emufp;                     // true FILE fp used for emulation in Windows. In true EFI it is the CDEFILE fp
    //long   fpos;                      // fpos -> what ftell returns relative to SEEK_SET
    char* Buffer;                    // internal buffer
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
    size_t _osifWinNTFileWrite(CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile);
Description
    Write file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    void* ptr                   : buffer
    size_t nelem                : number of bytes
    CDEFILE* pCdeFile           : CDEFILE* file handle
Returns
    number of bytes written

**/
size_t _osifWinNTFileWrite(CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile) {
    //NOTE: filesize < 4GB    
    BOOL f;
    DWORD dwCount;

    f = WriteFile(
        (HANDLE)pCdeFile->emufp,        /*__in          HANDLE hFile,*/
        ptr,                            /*__in          LPCVOID lpBuffer,*/
        (DWORD)nelem,                   /*__in          DWORD nNumberOfBytesToWrite,*/
        &dwCount,                       /*__out         LPDWORD lpNumberOfBytesWritten,*/
        NULL                            /*__in          LPOVERLAPPED lpOverlapped*/
    );

    return f != 0 ? (size_t)dwCount : 0;
}
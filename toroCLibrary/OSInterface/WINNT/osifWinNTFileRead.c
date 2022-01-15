/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileRead.c

Abstract:

    OS interface (osif) read file Windows NT

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
    size_t _osifWinNTFileRead(IN CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile)
Description
    Read a file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    void* ptr                   : buffer
    size_t nelem                : number of bytes
    CDEFILE* pCdeFile           : CDEFILE* file handle
Returns
    CDEFILE*: success
    NULL    : failure
**/
size_t _osifWinNTFileRead(IN CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile)
{
    //NOTE: filesize < 4GB    
    BOOL f;
    DWORD dwCount;

    f = ReadFile(
        (HANDLE)pCdeFile->emufp,        /*__in          HANDLE hFile,*/
        ptr,                            /*__in          LPCVOID lpBuffer,*/
        (DWORD)nelem,                   /*__in          DWORD nNumberOfBytesToRead,*/
        &dwCount,                       /*__out         LPDWORD lpNumberOfBytesRead,*/
        NULL                            /*__in          LPOVERLAPPED lpOverlapped*/
    );

    return f != 0 ? (size_t)dwCount : 0;
}

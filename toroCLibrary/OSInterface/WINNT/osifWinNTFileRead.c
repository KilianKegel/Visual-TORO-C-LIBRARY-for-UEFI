/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
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
#include <CdeServices.h>


#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void

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

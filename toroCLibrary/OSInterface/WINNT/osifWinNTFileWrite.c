/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
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
#include <CdeServices.h>

typedef unsigned long long EFI_PHYSICAL_ADDRESS;
typedef unsigned long long UINT64;
typedef void EFI_FILE_PROTOCOL;


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
    //
    // if redirected of if not STDERR/STDOUT
    //
    int fIsConsole = (0 != ((O_CDESTDOUT | O_CDESTDERR) & pCdeFile->openmode)) && (0 == (O_CDEREDIR & pCdeFile->openmode));

    if ((0 == fIsConsole) || (0 == (pCdeFile->openmode & (_O_U16TEXT | _O_WTEXT))))
    {
        f = WriteFile(
            (HANDLE)pCdeFile->emufp,        /*__in          HANDLE hFile,*/
            ptr,                            /*__in          LPCVOID lpBuffer,*/
            (DWORD)nelem,                   /*__in          DWORD nNumberOfBytesToWrite,*/
            &dwCount,                       /*__out         LPDWORD lpNumberOfBytesWritten,*/
            NULL                            /*__in          LPOVERLAPPED lpOverlapped*/
        );
    } else {
        f = WriteConsoleW(
            (HANDLE)pCdeFile->emufp,
            ptr,
            (DWORD)nelem/sizeof(wchar_t),
            &dwCount,
            NULL
        );
    }

    return f != 0 ? (size_t)dwCount : 0;
}
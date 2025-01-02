/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTDirectoryCreate.c

Abstract:

    OS interface (osif) to create a directory

Author:

    Kilian Kegel

--*/
#include <stdbool.h>
#include <windows.h>
#include <errhandlingapi.h>
//#undef NMOFINE
#include <stdio.h>

#include <cde.h>

#ifndef NMOFINE
extern char* gEfiCallerBaseName;
#endif//NMOFINE

/*++
Synopsis
    #include <CdeServices.h>
    int _osifWinNTDirectoryCreate(void* pCdeAppIf, const char* strDirectoryName);
Description
    Create new directory, non-existance assumed

Paramters
    void* pCdeAppIf             : unused
    const char* strDirectoryName: directory name ASCII
Returns
    0: success
    errno : failure

*/
int _osifWinNTDirectoryCreate(void* pCdeAppIf, wchar_t* pwcsDirName)
{
    int nRetErrno = EXIT_SUCCESS;
    BOOL f = CreateDirectoryW(pwcsDirName, NULL);
    
    //
    // translet WinNT GetLastError()-code to errno-code
    //
    if (false == f) {
        //
        //"https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-"
        //
        DWORD dwLastError = GetLastError();

        switch (dwLastError) {
        case 3  :   nRetErrno = ENOENT;   //No such file or directory, GetLastError() 3 ERROR_FILE_NOT_FOUND
            break;
        case 19 :   nRetErrno = EACCES;  // Permission denied, GetLastError() 19
            break;

        }
    }

    return nRetErrno;
}
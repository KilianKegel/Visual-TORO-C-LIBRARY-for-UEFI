/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileRename.c

Abstract:

    OS interface (osif) to rename a file Windows NT


Author:

    Kilian Kegel

--*/
#include <windows.h>
#define ALIENOS
#include <time.h>
#include <errno.h>
#include <cde.h>
/**
Synopsis
    #include <CdeServices.h>
    int _osifWinNTFileRename(IN void* pCdeAppIf, wchar_t* pwcsOld, wchar_t* pwcsNew)
Description
    Rename a file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    wchar_t* pwcsOldName        : old name wide string
    wchar_t* pwcsNewName        : new name wide string
Returns
    CDEFILE*: success
    NULL    : failure
**/
int _osifWinNTFileRename(IN void* pCdeAppIf, wchar_t* pwcsOld, wchar_t* pwcsNew)
{
    int nRet;

    CDEMOFINE((MFNINF(1) "%S -> %S\n", pwcsOld, pwcsNew));

    nRet = MoveFileW(pwcsOld, pwcsNew);
    //
    // translet WinNT GetLastError()-code to errno-code
    //
    if (1 != nRet) {
        DWORD dwLastError = GetLastError();
        switch (dwLastError) {
        case 183:   errno = EEXIST; //errno -> 17 == File exists, GetLastError() 183
            break;
        case 2:     errno = ENOENT; //errno -> 2 == No such file or directory, GetLastError() 2
            break;
        }
    }

    CDEMOFINE((MFNINF(1) "MoveFileW() returned %d\n", nRet));

    return nRet;
}

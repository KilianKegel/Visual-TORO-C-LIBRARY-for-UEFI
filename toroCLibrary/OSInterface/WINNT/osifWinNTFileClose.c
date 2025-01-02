/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
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
#include <CdeServices.h>


#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void


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

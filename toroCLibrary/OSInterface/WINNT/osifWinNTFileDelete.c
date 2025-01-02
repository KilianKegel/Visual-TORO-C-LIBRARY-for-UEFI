/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileDelete.c

Abstract:

    OS interface (osif) to delete a file UEFI Shell

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <windows.h>

/**
Synopsis
    #include <CdeServices.h>
    int _osifWinNTFileDelete(CDE_APP_IF* pCdeAppIf, const char* filename, CDEFILE* pCdeFile);
Description
    Delete a file
Paramters
    CDE_APP_IF* pCdeAppIf       : unused
    const char* filename        : filename
    CDEFILE* pCdeFile           : CDEFILE* to file
Returns
    0   : success
    EOF : failure
**/
int _osifWinNTFileDelete(void* pCdeAppIf, const char* filename, void* pCdeFile)
{
    BOOL fRet = DeleteFile(filename); // return 0 on error!!!

    return 0 == fRet ? -1 : 0;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTGetDrvCwd.c

Abstract:

    OS interface (osif) get current working directory Windows NT

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <Windows.h>
#include <stdint.h>
#include <wchar.h>

/**

Synopsis
    #include <CdeServices.h>
    char* _osifWinNTGetDrvCwd(void* pCdeAppIf, char* pstrDrvCwdBuf);
Description
    Write file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    IN OUT char* pstrDrvCwdBuf  : buffer to hold directory name as narrow string
Returns
    SUCCESS: pointer to current working directory
    FAILURE: NULL

**/
char* _osifWinNTGetDrvCwd(/*IN*/ void* pCdeAppIf, /*IN OUT*/ char* pstrDrvCwdBuf)
{
    uint32_t n = GetCurrentDirectory(CDE_FILESYSNAME_SIZE_MAX, pstrDrvCwdBuf);

    return 0 == n ? NULL : pstrDrvCwdBuf;
}
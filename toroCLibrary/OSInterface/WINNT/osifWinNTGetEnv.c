/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTGetEnv.c

Abstract:

    OS interface (osif) get environment variable Windows NT

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <time.h>
/**

Synopsis
    #include <CdeServices.h>
    char* _osifWinNTGetEnv(IN void* pCdeAppIf, const char* szEnvar);
Description
    Get environment variable
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    const char* pszEnvar        : environment variable name
Returns
    SUCCESS: pointer to environment variable string
    FAILURE: NULL

**/
char* _osifWinNTGetEnv(IN void/*CDE_APP_IF*/* pCdeAppIf, const char* szEnvar/*environment variable name*/)
{

    static char buffer[32769];
    char* pRet = buffer;

    if (0 == GetEnvironmentVariable(szEnvar, buffer, sizeof(buffer)))
        pRet = NULL;


    return pRet;
}

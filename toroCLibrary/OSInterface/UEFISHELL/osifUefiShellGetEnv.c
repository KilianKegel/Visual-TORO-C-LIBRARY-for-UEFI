/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellGetEnv.c

Abstract:

    OS interface (osif) get environment variable UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <PiPei.h>

#include <Base.h>
#include <CdeServices.h>
#include <Protocol\Shell.h>
#include <cde.h>

#define ELC(x) sizeof(x) / sizeof(x[0])

extern EFI_SYSTEM_TABLE* _cdegST;

/**

Synopsis
    #include <CdeServices.h>
    char* _osifUefiShellGetEnv(IN CDE_APP_IF* pCdeAppIf, const char* pszEnvar);
Description
    Get environment variable
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    const char* pszEnvar        : environment variable name
Returns
    SUCCESS: pointer to environment variable string
    FAILURE: NULL

**/
char* _osifUefiShellGetEnv(IN CDE_APP_IF* pCdeAppIf, const char* pszEnvar/*environment variable name*/)
{
    static char buffer[512];
    wchar_t wbuffer[64];
    wchar_t* pwcsEnvar;
    unsigned i;
    EFI_SHELL_PROTOCOL* pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;

    for (i = 0; i < sizeof(buffer); i++)
        buffer[i] = 0;

    for (i = 0; i < ELC(wbuffer); i++)
    {
        wbuffer[i] = (wchar_t)pszEnvar[i];
        if ('\0' == wbuffer[i])
            break;
    }

    pwcsEnvar = (wchar_t*)pEfiShellProtocol->GetEnv((const wchar_t*)&wbuffer[0]);

    if (NULL != pwcsEnvar) {
        for (i = 0; i < sizeof(buffer); i++)
        {
            buffer[i] = (char)pwcsEnvar[i];

            if ('\0' == buffer[i])
                break;
        }
    }

    return NULL == pwcsEnvar ? NULL : buffer;
}
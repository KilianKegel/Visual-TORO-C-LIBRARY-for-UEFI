/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    systemCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Executes a command on the system shell.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void* __cdeGetAppIf (void);

/**

Synopsis
    #include <stdlib.h>
    int system(const char *szCmd);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/system-wsystem?view=msvc-160&viewFallbackFrom=vs-2019
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/system-wsystem?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/system-wsystem?view=msvc-160#return-value

**/
static int systemCDEABI(const char *szCmd){
    CDE_APP_IF    *pCdeAppIf = __cdeGetAppIf();
    int nRet;

    nRet = szCmd == NULL ? 1 : pCdeAppIf->pCdeServices->pCmdExec(pCdeAppIf, szCmd);

    return nRet;
}

MKCDEABI(system);
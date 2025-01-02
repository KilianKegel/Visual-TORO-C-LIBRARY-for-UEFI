/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrTokCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Finds the next token in a string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**
Synopsis
    #include <string.h>
    char* strtok(char* pszStr, const char* pszSet);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-strtok-l-wcstok-wcstok-l-mbstok-mbstok-l?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-strtok-l-wcstok-wcstok-l-mbstok-mbstok-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-strtok-l-wcstok-wcstok-l-mbstok-mbstok-l?view=msvc-160#return-value
**/
static ROMPARM_WCSSTRTOK ROMPARM = {    
    /*fForceToDataSeg       */ 1 ,\
    /*fWide                 */ sizeof(char) > 1 ,\
};

static char* strtokCDEABI(char* pszStr, const char* pszSet) {

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return (char*)pCdeAppIf->pCdeServices->pWcsStrTok(pCdeAppIf, pszStr, pszSet, &pCdeAppIf->pStrtokStaticInternal, &ROMPARM);

}

MKCDEABI(strtok);
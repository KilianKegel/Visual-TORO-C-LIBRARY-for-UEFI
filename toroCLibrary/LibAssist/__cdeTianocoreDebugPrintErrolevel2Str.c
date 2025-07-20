/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeTianocoreDebugPrintErrolevel2Str.c

Abstract:

    DEBUG replacement for CdePkg helper function.
    Converts EDK2 error level to text string equivalent.

Author:

    Kilian Kegel

--*/
#include <cde.h>
#include <CdeServices.h>

/**

Synopsis
    char* __cdeTianocoreDebugPrintErrolevel2Str(size_t ErrorLevel,const char* Format, ...);
Description
    DEBUG replacement for CdePkg helper function.
    Convert PcdDebugPrintErrorLevel to its string equivalent
Returns

    @param[in]  ErrorLevel
    @param[in]  (ignored)Format

    @retval pointer to ASCII string "DEBUG_INIT>", "DEBUG_WARN>" ...

**/
char* __cdeTianocoreDebugPrintErrolevel2Str(size_t ErrorLevel,const char* Format, ...)
{
    char *pRet = "DEBUG_UNKNOWN>";
    switch (ErrorLevel) 
    {
    case DEBUG_INIT:        pRet = "DEBUG_INIT>"; break;
    case DEBUG_WARN:        pRet = "DEBUG_WARN>"; break;
    case DEBUG_LOAD:        pRet = "DEBUG_LOAD>"; break;
    case DEBUG_FS:          pRet = "DEBUG_FS>"; break;
    case DEBUG_POOL:        pRet = "DEBUG_POOL>"; break;
    case DEBUG_PAGE:        pRet = "DEBUG_PAGE>"; break;
    case DEBUG_INFO:        pRet = "DEBUG_INFO>"; break;
    case DEBUG_DISPATCH:    pRet = "DEBUG_DISPATCH>"; break;
    case DEBUG_VARIABLE:    pRet = "DEBUG_VARIABLE>"; break;
    case DEBUG_BM:          pRet = "DEBUG_BM>"; break;
    case DEBUG_BLKIO:       pRet = "DEBUG_BLKIO>"; break;
    case DEBUG_NET:         pRet = "DEBUG_NET>"; break;
    case DEBUG_UNDI:        pRet = "DEBUG_UNDI>"; break;
    case DEBUG_LOADFILE:    pRet = "DEBUG_LOADFILE>"; break;
    case DEBUG_EVENT:       pRet = "DEBUG_EVENT>"; break;
    case DEBUG_GCD:         pRet = "DEBUG_GCD>"; break;
    case DEBUG_CACHE:       pRet = "DEBUG_CACHE>"; break;
    case DEBUG_VERBOSE:     pRet = "DEBUG_VERBOSE>"; break;
    case DEBUG_ERROR:       pRet = "DEBUG_ERROR>"; break;
    }

    return pRet;

}


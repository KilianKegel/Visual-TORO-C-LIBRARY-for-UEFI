/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreSprintf.c

Abstract:

    sprintf() for core mode (that is: CDE_APP_IF* pCdeAppIf is parameter)

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdarg.h>
#include <limits.h>

extern VWXPRINTF _cdeCoreVwxPrintf;

int _cdeCoreSprintf(CDE_APP_IF* pCdeAppIf, char* pszDest, const char* pszFormat, ...) {
    va_list ap;
    unsigned nRet = UINT_MAX;
    size_t dwCount = (size_t)INT_MAX;
    unsigned dwUsedCount = (unsigned)dwCount;
    void (*pfnPutXxx)(int c, void** ppDest) = &_CdeMemPutChar;
    static ROMPARM_VWXPRINTF RomParm = { \
        /*fForceToDataSeg       */ 1 ,\
        /*fPointerIsParm        */ 1 ,\
        /*fPointerIsFilePointer */ 0 ,\
        /*fCountIsParm;         */ 1 ,\
        /*fAjustDifference      */ 0 ,\
        /*fWide                 */ 0 ,\
        /*fUEFIFormat           */ 0, \
    };

    va_start(ap, pszFormat);

    nRet = (int)_cdeCoreVwxPrintf(
        pCdeAppIf,
        &RomParm,           //IN ROMPARM_VWXPRINTF *pRomParm,
        pszFormat,          //IN const void *pszFormat,
        pfnPutXxx,          //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
        pszDest,            //unsigned char *pDest,
        (unsigned)dwUsedCount,    //unsigned dwCount,
        ap                  //IN va_list ap
        );
    if (dwUsedCount > nRet && nRet != UINT_MAX)
        pszDest[nRet] = '\0';

    va_end(ap);

    return (int)nRet;
}


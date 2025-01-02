/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    VsnprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Write formatted output using a pointer to a list of arguments.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <limits.h>

extern void* __cdeGetAppIf(void);
extern void _CdeMemPutChar(int c, void** ppDest);
extern void _CdeMemPutNada(int c, void** ppDest);
extern __declspec(dllimport) int vsnprintf(char* pszDest, size_t dwCount, const char* pszFormat, va_list ap);

static ROMPARM_VWXPRINTF RomParm = { \
/*fForceToDataSeg       */ 1 ,\
/*fPointerIsParm        */ 1 ,\
/*fPointerIsFilePointer */ 0 ,\
/*fCountIsParm;         */ 1 ,\
/*fAjustDifference      */ 0 ,\
/*fWide                 */ 0 ,\
/*fUEFIFormat           */ 0, \
};

/**
Synopsis
    #include <stdio.h>
    int vsnprintf(char* pszDest, size_t dwCount, const char* pszFormat, va_list ap);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-160#return-value
**/
static int vsnprintfCDEABI(char* pszDest, size_t dwCount, const char* pszFormat, va_list ap) {

    //NOTE: dwCount == INT_MAX -> buffer unlimited sprintf() is invoked w/o snprintf() constant return value of (theoretical) length
    unsigned nRet = UINT_MAX;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    unsigned dwUsedCount = (unsigned)dwCount;
    void (*pfnPutXxx)(int c, void** ppDest) = &_CdeMemPutChar;

    do {

        if (pszDest == NULL)
            if (dwCount == 0)
                dwUsedCount = UINT_MAX, pfnPutXxx = &_CdeMemPutNada;  // count only, don't write
            else
                break;

        nRet = (int)pCdeAppIf->pCdeServices->pVwxPrintf(
            pCdeAppIf,
            &RomParm,               //IN ROMPARM_VWXPRINTF *pRomParm,
            pszFormat,              //IN const void *pszFormat,
            pfnPutXxx,              //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
            pszDest,                //unsigned char *pDest,
            (unsigned)dwUsedCount,  //unsigned dwCount,
            ap                      //IN va_list ap
        );
        if (pszDest != NULL) {
            if (dwUsedCount > nRet && nRet != UINT_MAX)
                pszDest[nRet] = '\0';
            if (dwCount != INT_MAX && (nRet == UINT_MAX || dwCount == nRet)) {
                if (dwCount)
                    pszDest[dwCount - 1] = '\0';
                nRet = vsnprintf(NULL, 0, pszFormat, ap);
            }
        }

    } while (0);

    return (int)nRet;
}

MKCDEABI(vsnprintf);
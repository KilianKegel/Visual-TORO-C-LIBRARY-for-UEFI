/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Sscanf.c

Abstract:

    Implementation of the Standard C function.
    Read formatted data from a string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

extern void* __cdeGetAppIf(void);
extern int _cdeMemGetChar(void** ppSrc);
extern int _cdeMemUngetChar(int c, void** ppSrc);

static ROMPARM_VWXSCANF RomParm = { \
/*fForceToDataSeg       */ 1 ,\
/*fPointerIsParm        */ 1 ,\
/*fPointerIsFilePointer */ 0 ,\
/*fCountIsParm;         */ 0 ,\
/*fAjustDifference      */ 0 ,\
/*fWide                 */ 0 ,\
/*EndMarker             */'\0'\
};
/**

Synopsis
    #include <stdio.h>
    int sscanf(const char* pszBuffer, const char* pszFormat, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-160#return-value
**/
int sscanf(const char* pszBuffer, const char* pszFormat, ...) {
    va_list ap;
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    va_start(ap, pszFormat);


    nRet = (int)pCdeAppIf->pCdeServices->pVwxScanf(
        pCdeAppIf,                  //this
        &RomParm,                   //IN ROMPARM_VWXPRINTF *pRomParm,
        pszFormat,                  //IN const void *pszFormat,
        &_cdeMemGetChar,            //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
        &_cdeMemUngetChar,          //int (*pfnDevUngetChar)(int c, void** ppDst)
        (unsigned char*)pszBuffer,  //unsigned char *pDest,
        (unsigned)-1,               //unsigned dwCount,
        NULL,                       //ppEnd
        ap                          //IN va_list ap
    );
    va_end(ap);

    return nRet;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeSwscanf.c

Abstract:

    Toro C Library specific implementation
    Read formatted data from a wide string and return pointer to character that terminates the wide string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

extern void* __cdeGetAppIf(void);
extern int _cdeMemGetWChar(void** ppSrc);
extern int _cdeMemUngetWChar(int c, void** ppSrc);

static ROMPARM_VWXSCANF RomParm = { \
/*fForceToDataSeg       */ 1 ,\
/*fPointerIsParm        */ 1 ,\
/*fPointerIsFilePointer */ 0 ,\
/*fCountIsParm;         */ 0 ,\
/*fAjustDifference      */ 0 ,\
/*fWide                 */ 1 ,\
/*EndMarker             */'\0'\
};


/**

Synopsis
    int _cdeSscanf(const char* pszBuffer, char** endptr, const char* pszFormat, ...);
Description
    Function processes formatted string as sscanf() does, but additionally 
    provides e entptr.
Paramaters
    IN const char* pszBuffer
    IN char** endptr
    IN const char* pszFormat
    OUT variadic arguments likely to sscanf()
Returns
    number of tokens

**/
int _cdeSwscanf(const wchar_t* pwcsBuffer, wchar_t** endptr, const wchar_t* pwcsFormat, ...) {
    va_list ap;
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    va_start(ap, pwcsFormat);

    nRet = (int)pCdeAppIf->pCdeServices->pVwxScanf(
        pCdeAppIf,                  //this
        &RomParm,                   //IN ROMPARM_VWXPRINTF *pRomParm,
        (void*)pwcsFormat,          //IN const void *pwcsFormat,
        &_cdeMemGetWChar,           //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
        &_cdeMemUngetWChar,         //int (*pfnDevUngetChar)(int c, void** ppDst)
        (unsigned char*)pwcsBuffer, //unsigned char *pDest,
        (unsigned)-1,               //unsigned dwCount,
        (unsigned char **)endptr,   //ppEnd
        ap                          //IN va_list ap
    );
    va_end(ap);

    return nRet;
}
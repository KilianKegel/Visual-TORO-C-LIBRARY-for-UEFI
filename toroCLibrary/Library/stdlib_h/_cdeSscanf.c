/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeSscanf.c

Abstract:

    Toro C Library specific implementation
    Read formatted data from a string and return pointer to character that terminates the string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

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
int _cdeSscanf(const char* pszBuffer, char** endptr, const char* pszFormat, ...) {
    va_list ap;
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    va_start(ap, pszFormat);

    //printf("pszFormat == %p, ap == %p\n",pszFormat,ap);
    nRet = (int)pCdeAppIf->pCdeServices->pVwxScanf(
        pCdeAppIf,                  //this
        &RomParm,                   //IN ROMPARM_VWXPRINTF *pRomParm,
        pszFormat,                  //IN const void *pszFormat,
        &_cdeMemGetChar,            //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
        &_cdeMemUngetChar,          //int (*pfnDevUngetChar)(int c, void** ppDst)
        (unsigned char*)pszBuffer,  //unsigned char *pDest,
        (unsigned)-1,               //unsigned dwCount,
        (unsigned char**)endptr,    //ppEnd
        ap                          //IN va_list ap
    );
    va_end(ap);

    return nRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Vsscanf.c

Abstract:

    Implementation of the Standard C function.
    Reads formatted data from a string.

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
    #include <stdio.h>
    int vsscanf(const char* pszBuffer, const char* pszFormat, va_list ap) {;
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsscanf-vswscanf?view=msvc-160&viewFallbackFrom=vs-2019
    The vsscanf function is equivalent to sscanf, with the variable argument list
    replaced by arg, which shall have been initialized by the va_start macro (and
    possibly subsequent va_arg calls). The vsscanf function  does  not  invoke  the
    va_end macro.
Returns
    The vsscanf function  returns  the  value  of  the  macro EOF
    if  an  input  failure  occurs before  any  conversion. Otherwise,
    the vsscanf function  returns  the  number  of  input items assigned,
    which can be fewer than provided for, or even zero, in the event of an
    early matching failure.
**/
int vsscanf(const char* pszBuffer, const char* pszFormat, va_list ap) {
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

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
    return nRet;
}

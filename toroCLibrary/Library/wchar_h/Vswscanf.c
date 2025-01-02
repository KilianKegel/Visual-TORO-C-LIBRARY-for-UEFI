/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Vswscanf.c

Abstract:

    Implementation of the Standard C function.
    Reads formatted data from a wide string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

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
#include <wchar.h>
    #include <stdarg.h>
    #include <wchar.h>
    int vswscanf(const wchar_t * restrict s,
    const wchar_t * restrict format,va_list arg);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsscanf-vswscanf?view=msvc-170
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=377
    The vswscanf function is equivalent to swscanf, with the variable argument list
    replaced by arg, which shall have been initialized by the va_start macro (and
    possibly subsequent va_arg calls). The vswscanf function does not invoke the
    va_end macro.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-170#return-value
**/
int vswscanf(const wchar_t* pwcsBuffer, const wchar_t* pwcsFormat, va_list ap) {
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    nRet = (int)pCdeAppIf->pCdeServices->pVwxScanf(
        pCdeAppIf,                  //this
        &RomParm,                   //IN ROMPARM_VWXPRINTF *pRomParm,
        (void*)pwcsFormat,          //IN const void *pwcsFormat,
        &_cdeMemGetWChar,           //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
        &_cdeMemUngetWChar,         //int (*pfnDevUngetChar)(int c, void** ppDst)
        (unsigned char*)pwcsBuffer, //unsigned char *pDest,
        (unsigned)-1,               //unsigned dwCount,
        NULL,                       //ppEnd
        ap                          //IN va_list ap
    );

    return nRet;
}

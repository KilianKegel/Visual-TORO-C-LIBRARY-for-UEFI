/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    VfwscanfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Reads formatted data from a wide string.

Author:

    Kilian Kegel

--*/

#include <cde.h>
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
#define WEOF ((wint_t)(0xFFFF))
#define FILE void

extern void* __cdeGetAppIf(void);
extern __declspec(dllimport) wint_t fgetwc(FILE* stream);
extern __declspec(dllimport) wint_t ungetwc(wint_t c, FILE* stream);

static ROMPARM_VWXSCANF RomParm = { \
/*fForceToDataSeg       */ 1 ,\
/*fPointerIsParm        */ 1 ,\
/*fPointerIsFilePointer */ 1 ,\
/*fCountIsParm;         */ 0 ,\
/*fAjustDifference      */ 0 ,\
/*fWide                 */ 1 ,\
/*EndMarker             */WEOF \
};

static int __fgetwcpp(void** stream)
{
    return fgetwc(*stream);
}

static int __fungetwcpp(int c, void** stream)
{
    CDEFILE* pCdeFile = *stream;
    int nRet = RomParm.EndMarker;                       // NOTE: check UngetCharAndCountDown() for understanding

    do {
        //
        // in O_TEXT mode don't push back a '\0' character
        //
        if (O_TEXT == (pCdeFile->openmode & O_TEXT))
        {
            if ('\0' != c)
            {
                nRet = ungetwc((wint_t)c, *stream);     // NOTE ungetc() return EOF, not WEOF
            }
            break;
        }

        nRet = ungetwc((wint_t)c, *stream);

    } while (0);

    nRet = EOF == nRet ? RomParm.EndMarker : nRet;

    return nRet;
//    return ungetwc((wint_t)c, *stream);
}

/**

Synopsis
    #include <wchar.h>
    #include <stdarg.h>
    #include <stdio.h>
    #include <wchar.h>
    int vfwscanf(FILE * restrict stream, const wchar_t * restrict format, va_list arg);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfscanf-vfwscanf?view=msvc-170
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=376
    The vfwscanf function is equivalent to fwscanf, with the variable argument list
    replaced by arg, which shall have been initialized by the va_start macro (and
    possibly subsequent va_arg calls). The vfwscanf function does not invoke the
    va_end macro.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfscanf-vfwscanf?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfscanf-vfwscanf?view=msvc-170#return-value
**/
static int vfwscanfCDEABI(FILE* stream, const wchar_t* pwcsFormat, va_list ap) {
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    nRet = (int)pCdeAppIf->pCdeServices->pVwxScanf(
        pCdeAppIf,                  //this
        &RomParm,                   //IN ROMPARM_VWXPRINTF *pRomParm,
        (void*)pwcsFormat,          //IN const void *pwcsFormat,
        &__fgetwcpp,                //void (*pfnDevPutChar)(unsigned short/*wchar_t*/c,void** ppDest/*address of pDest*/),
        &__fungetwcpp,              //int (*pfnDevUngetChar)(int c, void** ppDst)
        (unsigned char*)stream,     //UINT8 *pDest,
        (unsigned)-1,               //unsigned dwCount,
        NULL,                       //ppEnd
        ap                          //IN va_list ap
    );

    return nRet;
}

MKCDEABI(vfwscanf);
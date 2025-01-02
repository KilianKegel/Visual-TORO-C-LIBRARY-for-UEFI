/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Vfscanf.c

Abstract:

    Implementation of the Standard C function.
    Write formatted output using a pointer to a list of arguments.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>

#pragma warning (disable:4028)

extern void* __cdeGetAppIf(void);
extern int fgetc(FILE* stream);
extern int ungetc(int c, FILE* stream);

static ROMPARM_VWXSCANF RomParm = { \
/*fForceToDataSeg       */ 1 ,\
/*fPointerIsParm        */ 1 ,\
/*fPointerIsFilePointer */ 1 ,\
/*fCountIsParm;         */ 0 ,\
/*fAjustDifference      */ 0 ,\
/*fWide                 */ 0 ,\
/*EndMarker             */EOF \
};

static int __fgetcpp(void** stream)
{
    return fgetc(*stream);
}

static int __fungetcpp(int c, void** stream)
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
                nRet = ungetc(c, *stream);              // NOTE ungetc() return EOF, not WEOF
            }
            break;
        }

        nRet = ungetc(c, *stream);

    } while (0);
        
    nRet = EOF == nRet ? RomParm.EndMarker : nRet;

    return nRet;
}

/**
Synopsis
    #include <stdio.h>
    int vfscanf(FILE* stream, const char* pszFormat, va_list ap);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfscanf-vfwscanf?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=305
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfscanf-vfwscanf?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfscanf-vfwscanf?view=msvc-160#return-value
**/
int vfscanf(FILE* stream, const char* pszFormat, va_list ap) {
    int nRet;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    nRet = (int)pCdeAppIf->pCdeServices->pVwxScanf(
        pCdeAppIf,              //this
        &RomParm,               //IN ROMPARM_VWXPRINTF *pRomParm,
        pszFormat,              //IN const void *pszFormat,
        &__fgetcpp,             //int (*pfnDevGetChar)(void** ppSrc),
        &__fungetcpp,           //int (*pfnDevUngetChar)(int c, void** ppDst)
        (unsigned char*)stream, //UINT8 *pDest,
        (unsigned)-1,           //unsigned dwCount,
        NULL,                   //ppEnd
        ap                      //IN va_list ap
    );
    return nRet;
}

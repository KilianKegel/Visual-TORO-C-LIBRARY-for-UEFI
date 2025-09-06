/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Vfprintf.c

Abstract:

    Implementation of the Standard C function.
    Write formatted output using a pointer to a list of arguments.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <errno.h>

#include <cde.h>
#include <CdeServices.h>

extern void* __cdeGetAppIf(void);

static void __fputcpp(int b, void** pstream) {
    fputc(b, *pstream);
}
/**
Synopsis
    #include <stdio.h>
    int vfprintf(FILE *stream, const char *format, va_list argptr);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfprintf-vfprintf-l-vfwprintf-vfwprintf-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfprintf-vfprintf-l-vfwprintf-vfwprintf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vfprintf-vfprintf-l-vfwprintf-vfwprintf-l?view=msvc-160#return-value
**/
int vfprintf(FILE* const stream, char const* const pszFormat, va_list ap) {
    ROMPARM_VWXPRINTF RomParm = { \
        /*fForceToDataSeg       */ 1 ,\
        /*fPointerIsParm        */ 0 ,\
        /*fPointerIsFilePointer */ 1 ,\
        /*fCountIsParm;         */ 0 ,\
        /*fAjustDifference      */ 0 ,\
        /*fWide                 */ 0 ,\
        /*fUEFIFormat           */ 0, \
    };
    int nRet = 0;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    void* pDest = stream;
    
    if (NULL == pCdeAppIf)
        errno = EPERM;
    else do
    {
        _PUTCHAR* pfnOutput = NULL;

        if (CDEDBGMAGIC == (size_t)stream)
        {
            pfnOutput = pCdeAppIf->pCdeServices->pPutDebug;
            pDest = (void*)pCdeAppIf;
        }
        else 
        {
            switch (pCdeAppIf->DriverParm.CommParm.OSIf)
            {
            case PEIIF:
            case DXEIF:
            case SMMIF:
                if (    (FILE*)CDE_STDOUT == stream 
                    ||  (FILE*)CDE_STDERR == stream)
                    pfnOutput = pCdeAppIf->pCdeServices->pPutConOut;
                    
                pDest = (void*)pCdeAppIf;

                break;
            default:
                pfnOutput = __fputcpp;
                break;
            }
        }
    nRet = (int)pCdeAppIf->pCdeServices->pVwxPrintf(
            pCdeAppIf,          // this
            &RomParm,           // IN ROMPARM_VWXPRINTF *pRomParm,
            pszFormat,          // IN const void *pszFormat,
            pfnOutput,          // void (*pfnDevPutChar)(UINT16/*wchar_t*/c,void** ppDest/*address of pDest*/),
            pDest,              // UINT8 *pDest, pointer for the output function memory address or pCdeAppIf
            (unsigned)-1,       // unsigned dwCount,
            ap                  // IN va_list ap
        );

        if (stdout == pDest) {
            pCdeAppIf->pCdeServices->pPutConOut(EOF, (void**)&pCdeAppIf);           //flush to stdout
        }

    } while (0);        

    return nRet;
}
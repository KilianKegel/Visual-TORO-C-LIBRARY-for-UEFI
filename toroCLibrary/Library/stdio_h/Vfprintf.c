/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
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
extern void _cdeInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);

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
    static ROMPARM_VWXPRINTF RomParmSHELL = {
        .fForceToDataSeg = 1,
        .fCdeFilePointer = 1,   // after POST it is a pCdeFile
        .fCountIsParm = 0,
        .fAjustDifference = 0,
        .fWide = 0,
        .fUEFIFormat = 0
    };
    static ROMPARM_VWXPRINTF RomParmPOST = {
        .fForceToDataSeg = 1,
        .fCdeFilePointer = 0,   // during POST it is a pCdeAppIf
        .fCountIsParm = 0,
        .fAjustDifference = 0,
        .fWide = 0,
        .fUEFIFormat = 0
    };

    int nRet = 0;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    void* pDest = stream;
    ROMPARM_VWXPRINTF* pROMPARM_VWXPRINTF = &RomParmPOST;

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
                if ((FILE*)CDE_STDOUT == stream
                    || (FILE*)CDE_STDERR == stream)
                    pfnOutput = pCdeAppIf->pCdeServices->pPutConOut;

                pDest = (void*)pCdeAppIf;

                break;
            default:
                pfnOutput = __fputcpp;
                pROMPARM_VWXPRINTF = &RomParmSHELL;
                break;
            }
        }

        //
        //  check to invoke invalid parameter handler when file stream is 
        //
        if (TRUE == pROMPARM_VWXPRINTF->fCdeFilePointer)
            if (0 == (O_CDEREDIR & ((CDEFILE*)pDest)->openmode))
                if (0 != ((_O_U16TEXT | _O_WTEXT) & ((CDEFILE*)pDest)->openmode))
                    _cdeInvalidParameterHandler(NULL, NULL, NULL, 0, 0);

        nRet = (int)pCdeAppIf->pCdeServices->pVwxPrintf(
            pCdeAppIf,          // this
            pROMPARM_VWXPRINTF, // IN ROMPARM_VWXPRINTF *pRomParm,
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
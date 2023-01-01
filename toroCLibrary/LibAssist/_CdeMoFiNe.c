/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeMoFiNe.c

Abstract:

    CDE Trace support

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>
#include <CDE.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>

int _CdeMofine(char* pszDriver, char* pszFile, int nLine, char* pszFunction, char* pszClass, int fTraceEn, char* pszFormat, ...) {
    int nRet = 0;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    if (pCdeAppIf) do {
        if ((fTraceEn & 1) == 1)
        {
            va_list ap;

            va_start(ap, pszFormat);

            nRet = pCdeAppIf->pCdeServices->pVMofine(pCdeAppIf, pszDriver, pszFile, nLine, pszFunction, pszClass, fTraceEn, pszFormat, ap);

            va_end(ap);

        }//if(fTraceEn == 1;
    } while (0);

    return nRet;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreMofine.c

Abstract:

    MOduleFIleliNE() trace for core mode (that is: CDE_APP_IF* pCdeAppIf is parameter)

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdarg.h>
#include <limits.h>

int _cdeCoreMofine(CDE_APP_IF* pCdeAppIf, char* pszDriver, char* pszFile, int nLine, char* pszFunction, char* pszClass, int fTraceEn, char* pszFormat, ...) {
    int nRet = 0;

    va_list ap;

    va_start(ap, pszFormat);

    nRet = pCdeAppIf->pCdeServices->pVMofine(pCdeAppIf, pszDriver, pszFile, nLine, pszFunction, pszClass, fTraceEn, pszFormat, ap);

    va_end(ap);

    return nRet;
}

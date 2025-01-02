/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _efierrno.c

Abstract:

    Adaption of the Standard C function errno() to toro C Library specific _efierrno()

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
CDE_STATUS/*EFI_STATUS*/ * _efierrno(void)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    CDE_STATUS* pRet = NULL;

    if (NULL != pCdeAppIf)
        pRet = &pCdeAppIf->nEfiErrno;

    return pRet;
}
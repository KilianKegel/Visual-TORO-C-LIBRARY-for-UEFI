/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __iob_func.c

Abstract:

    Microsoft specific intrinsic function used by compilers before VS2015
    The __iob_func() function retrieves the address of a given
    I/O buffer index.

Author:

    Kilian Kegel

--*/
#include <errno.h>
#include <CdeServices.h>

CDEFILE* __iob_func(void)
{
    CDEFILE* pRet = NULL;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    if (NULL == pCdeAppIf)
        errno = EPERM;
    else 
        pRet = &pCdeAppIf->pIob[0];

    return pRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeOnErrSet_errnoCORE.c

Abstract:

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// errno.h
//
#undef errno
#define errno (*_errnoCORE(pCdeAppIf))

extern int* _errnoCORE(CDE_APP_IF* pCdeAppIf);

int __cdeOnErrSet_errnoCORE(CDE_APP_IF *pCdeAppIf, CDE_STATUS Status, int Error) {

    if (CDE_SUCCESS != Status)
        errno = Error;

    return Error;
}

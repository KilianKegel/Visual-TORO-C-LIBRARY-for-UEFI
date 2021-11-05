/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeOnErrSet_errno.c

Abstract:

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <errno.h>

int __cdeOnErrSet_errno(CDE_STATUS Status, int Error) {

    if (CDE_SUCCESS != Status)
        errno = Error;

    return Error;
}

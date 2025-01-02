/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeOnErrSet_errno.c

Abstract:

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
//
// CdeServices.h
//
#if   defined(_M_AMD64)
    typedef unsigned long long size_t;
#else
    typedef unsigned int size_t;
#endif
#define CDE_STATUS size_t
#define CDE_SUCCESS 0
//
// errno.h
//
#define errno (*_errno())

extern __declspec(dllimport) int* _errno(void);

int __cdeOnErrSet_errno(CDE_STATUS Status, int Error) {

    if (CDE_SUCCESS != Status)
        errno = Error;

    return Error;
}

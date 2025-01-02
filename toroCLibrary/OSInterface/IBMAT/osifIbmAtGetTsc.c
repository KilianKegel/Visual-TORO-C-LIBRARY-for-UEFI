/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifIbmAtGetTsc.c

Abstract:

    OS interface (osif) to get time stamp counter

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/** _osifGetTsc() - get time stamp counter

Synopsis
    unsigned long long _osifIbmAtGetTsc(IN CDE_APP_IF* pCdeAppIf);
Description
    get TSC
Parameters
    CDE_APP_IF* pCdeAppIf
Returns
    TSC
**/
unsigned long long _osifIbmAtGetTsc(IN CDE_APP_IF* pCdeAppIf) {

    return __rdtsc();
}
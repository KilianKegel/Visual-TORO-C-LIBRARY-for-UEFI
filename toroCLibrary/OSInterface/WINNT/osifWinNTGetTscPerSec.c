/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTGetTscPerSec.c

Abstract:

    OS interface (osif) to get PerformanceCounter frequency on Windows NT

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <time.h>

/** _osifGetTscPerSec() - get time stamp counter frequency

Synopsis
    UINT64 _osifWinNTGetTscPerSec(IN void* pCdeAppIf);
Description
    get TSC frequency
Parameters
    CDE_APP_IF* pCdeAppIf
Returns
    PerformanceCounter frequency
**/
UINT64 _osifWinNTGetTscPerSec(IN void/*CDE_APP_IF*/* pCdeAppIf) {

    LARGE_INTEGER Frequency;
    UINT64* pRet = (UINT64*)&Frequency;

    QueryPerformanceFrequency(&Frequency);

    return *pRet;
}

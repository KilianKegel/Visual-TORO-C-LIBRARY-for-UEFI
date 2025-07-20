/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTGetTsc.c

Abstract:

    OS interface (osif) to get time stamp counter Windows NT
    NOTE:   On Windows the PerformanceCounter is used instead.
            TSC unusable in a multitasking OS

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <time.h>

/** _osifGetTsc() - get time stamp counter

Synopsis
    UINT64 _osifWinNTGetTsc(IN void* pCdeAppIf);
Description
    get TSC
Parameters
    CDE_APP_IF* pCdeAppIf
Returns
    PerformanceCounter
**/
UINT64 _osifWinNTGetTsc(IN void/*CDE_APP_IF*/* pCdeAppIf) {

    LARGE_INTEGER PerfClk;
    UINT64* pRet = (UINT64*)&PerfClk;


    QueryPerformanceCounter(&PerfClk);

    return *pRet;

}

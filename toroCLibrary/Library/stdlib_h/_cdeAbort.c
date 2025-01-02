/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeAbort.c

Abstract:

    Toro C Library specific signal default handler.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdlib.h
//
extern __declspec(dllimport) void _exit(int status);
/**

Synopsis
    void _cdeAbort(void);
Description
    Toro C Library internal abort function invoked by the default signal handler.
    It _doesn't_ write "abnormal program termination" to stderr
    and calls the exit(3) function.
Returns
    does not return
**/
void _cdeAbort(void) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int i;

    //
    // prevent all atexit-registered functions from being called
    //
    for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--)
        pCdeAppIf->rgcbAtexit[i] = NULL;

    //
    // prevent all file buffers from being flushed
    //
    for (i = 0; i < pCdeAppIf->cIob; i++)
        pCdeAppIf->pIob[i].fRsv = 0;

    _exit(0xC0000409/*STATUS_STACK_BUFFER_OVERRUN*/); //NOTE: Returnvalue of 3 documented by Microsoft instead
}

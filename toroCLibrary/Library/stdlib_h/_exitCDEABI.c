/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _exit.c

Abstract:

    Implementation of the Microsoft C function.
    Terminates the calling process. Skip atexit-registered functions.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void* __cdeGetAppIf(void);
extern void longjmp(jmp_buf env, int val);
/**

Synopsis
    #include <stdlib.h>
    void _exit(int status);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit?view=msvc-160#parameters
Returns
    does not return
**/
static void _exitCDEABI(int status) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    //
    // prevent all atexit-registered functions from being called
    //
    for (int i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--)
        pCdeAppIf->rgcbAtexit[i] = NULL;

    //
    // prevent all file buffers from being flushed
    //
    for (int i = 0; i < pCdeAppIf->cIob; i++)
        pCdeAppIf->pIob[i].fRsv = 0;

    pCdeAppIf->exit_status = status;
    longjmp(pCdeAppIf->exit_buf, status);
}

MKCDEABI(_exit);
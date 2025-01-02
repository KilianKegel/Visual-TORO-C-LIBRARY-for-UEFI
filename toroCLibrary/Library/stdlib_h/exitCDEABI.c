/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    exit.c

Abstract:

    Implementation of the Standard C function.
    Terminates the calling process. The exit function terminates it after cleanup.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void* __cdeGetAppIf(void);
extern __declspec(dllimport) void longjmp(jmp_buf env, int val);
/**

Synopsis
    #include <stdlib.h>
    void exit(int status);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit?view=msvc-160#parameters
Returns
    does not return
**/
static void exitCDEABI(int status) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    pCdeAppIf->exit_status = status;
    longjmp(pCdeAppIf->exit_buf, status);
}

MKCDEABI(exit);

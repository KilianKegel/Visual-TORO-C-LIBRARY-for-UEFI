/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    atexitCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Processes the specified function at exit.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    int atexit(void (*func)(void));
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atexit?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atexit?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atexit?view=msvc-160#return-value

**/
static int atexitCDEABI(void (*func)(void)) {

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int i;

    for (i = 0; i < CDE_ATEXIT_REGISTRATION_NUM; i++) {
        if (NULL == pCdeAppIf->rgcbAtexit[i]) {
            pCdeAppIf->rgcbAtexit[i] = func;
            break;
        }
    }
    return i == CDE_ATEXIT_REGISTRATION_NUM; //returns zero if successful
}

MKCDEABI(atexit);
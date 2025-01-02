/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _wassertCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) int printf(const char* pszFormat, ...);

static void __cdecl _wassertCDEABI(wchar_t const* pwcsExpession, wchar_t const* pwcsFile, unsigned dwLine) {

    if (CDE_REMOVEPATHFROMFILENAME) {
        int i = 0;

        while (pwcsFile[i++] != '\0')
            ;

        while (i > 0 && pwcsFile[i - 1] != '\\' && pwcsFile[i - 1] != '/')
            i--;

        pwcsFile = &pwcsFile[i];
    }


    printf("Assertion failed: %S, file %S, line %d\n", pwcsExpession, pwcsFile, dwLine);

}

MKCDEABI(_wassert);
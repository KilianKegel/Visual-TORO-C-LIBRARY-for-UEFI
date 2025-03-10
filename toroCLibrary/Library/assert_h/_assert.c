/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _assert.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern int printf(const char* pszFormat, ...);

void __cdecl _assert(char* pszExpession, char* pszFile, unsigned dwLine) {

    if (CDE_REMOVEPATHFROMFILENAME) {
        int i = 0;

        while (pszFile[i++] != '\0')
            ;

        while (i > 0 && pszFile[i - 1] != '\\' && pszFile[i - 1] != '/')
            i--;

        pszFile = &pszFile[i];
    }


    printf("Assertion failed: %s, file %s, line %d\n", pszExpession, pszFile, dwLine);

}
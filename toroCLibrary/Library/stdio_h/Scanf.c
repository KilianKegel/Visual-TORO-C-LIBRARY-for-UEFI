/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Scanf.c

Abstract:

    Implementation of the Standard C function.
    Reads formatted data from the standard input stream. 

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>
/**
Synopsis
    #include <stdio.h>
    int scanf(const char* pszFormat, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l?view=msvc-160&viewFallbackFrom=vs-2019
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l?view=msvc-160#return-value

**/
int scanf(char const* const pszFormat, ...) {
    va_list ap;
    int nRet;

    va_start(ap, pszFormat);


    nRet = vfscanf((FILE*)CDE_STDIN, pszFormat, ap);

    va_end(ap);

    return nRet;

}

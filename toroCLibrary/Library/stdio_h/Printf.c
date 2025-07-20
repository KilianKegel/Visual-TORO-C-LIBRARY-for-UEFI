/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Printf.c

Abstract:

    Implementation of the Standard C function.
    Prints formatted output to the standard output stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdio.h>

/**

Synopsis
    #include <stdio.h>
    int printf(const char* pszFormat, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-160&viewFallbackFrom=vs-2019
 Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-160#parameters
 Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-160#return-value
**/
int printf(char const* const pszFormat, ...)
{
    va_list ap;
    int nRet = 0;

    va_start(ap, pszFormat);

    nRet = vfprintf(stdout, pszFormat, ap);

    va_end(ap);

    return nRet;
}
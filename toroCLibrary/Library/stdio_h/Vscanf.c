/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Vscanf.c

Abstract:

    Implementation of the Standard C function.
    Writes formatted output by using a pointer to a list of arguments.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>

/**
Synopsis
    #include <stdio.h>
    int vscanf(const char* pszFormat, va_list ap);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vscanf-vwscanf?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vscanf-vwscanf?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vscanf-vwscanf?view=msvc-160#return-value
**/
int vscanf(char const* const  pszFormat, va_list ap) {
    return vfscanf((FILE*)CDE_STDIN, pszFormat, ap);
}

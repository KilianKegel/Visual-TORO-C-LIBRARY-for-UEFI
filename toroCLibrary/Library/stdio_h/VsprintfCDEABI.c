/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    VsprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Write formatted output using a pointer to a list of arguments.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <limits.h>

extern __declspec(dllimport) int vsnprintf(char* pszDest, size_t dwCount, const char* pszFormat, va_list ap);

/**
Synopsis
    #include <stdio.h>
    int vsprintf(char* pszDest, const char* pszFormat, va_list ap);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsprintf-vsprintf-l-vswprintf-vswprintf-l-vswprintf-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-160#return-value
**/
static int vsprintfCDEABI(char* pszDest, const char* pszFormat, va_list ap) {

    return vsnprintf(pszDest, (size_t)INT_MAX/*dwCount*/, pszFormat, ap);
}

MKCDEABI(vsprintf);
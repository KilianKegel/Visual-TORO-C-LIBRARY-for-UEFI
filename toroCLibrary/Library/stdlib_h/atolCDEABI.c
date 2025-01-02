/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    atolCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Convert a string to a long integer.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
extern __declspec(dllimport) int sscanf(char* pszBuffer, const char* pszFormat, ...);

/**

Synopsis
    long atol(const char* s);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atol-atol-l-wtol-wtol-l?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atol-atol-l-wtol-wtol-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atol-atol-l-wtol-wtol-l?view=msvc-160#return-value

**/
static long atolCDEABI(const char* s) {
    int l = 0;
    sscanf((char*)s, "%d", &l);
    return l;
}

MKCDEABI(atol);
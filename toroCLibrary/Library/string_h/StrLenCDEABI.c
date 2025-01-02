/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrLenCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Gets the length of a string.

Author:

    Kilian Kegel

--*/
#include <stddef.h>
#include <CdeServices.h>

/**
Synopsis
    #include <string.h>
    size_t strlen(const char* pszBuffer) ;
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strlen-wcslen-mbslen-mbslen-l-mbstrlen-mbstrlen-l?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strlen-wcslen-mbslen-mbslen-l-mbstrlen-mbstrlen-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strlen-wcslen-mbslen-mbslen-l-mbstrlen-mbstrlen-l?view=msvc-160#return-value
**/
static size_t strlenCDEABI(const char* pszBuffer) {
    int i = 0;

    while (pszBuffer[i] != '\0')
        i++;
    return (size_t)i;
}

MKCDEABI(strlen);

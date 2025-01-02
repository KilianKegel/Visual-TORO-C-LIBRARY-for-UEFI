/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrNLenCDEABI.c

Abstract:

    Import Library version

    Implementation of the Microsoft/POSIX C function.
    Gets the length of a string by using the current locale or one that has been passed in

Author:

    Kilian Kegel

--*/
#include <stddef.h>
#include <CdeServices.h>

/**
Synopsis
    #include <string.h>
    size_t strnlen(const char* pszBuffer, size_t n) ;
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnlen-strnlen-s?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnlen-strnlen-s?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnlen-strnlen-s?view=msvc-160#return-value
**/
static size_t strnlenCDEABI(const char* pszBuffer, size_t n) {
    size_t i = 0;

    while (pszBuffer[i] != '\0')
        if (i == n)
            break;
        else
            i++;

    return i;
}

MKCDEABI(strnlen);
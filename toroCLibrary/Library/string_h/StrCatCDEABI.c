/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrCatCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Appends a string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);

/**

Synopsis
    #include <string.h>
    char* strcat(char* s1, const char* s2);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcat-wcscat-mbscat?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcat-wcscat-mbscat?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcat-wcscat-mbscat?view=msvc-160#return-value
**/
static char* strcatCDEABI(char* s1, const char* s2) {
    strcpy(&s1[strlen(s1)], s2);
    return s1;
}

MKCDEABI(strcat);
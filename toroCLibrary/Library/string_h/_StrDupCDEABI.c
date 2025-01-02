/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _StrDupCDEABI.c

Abstract:

    Import Library version

    Implementation of the POSIX C function.
    Duplicates strings.

Author:

    Kilian Kegel

--*/
#include <stddef.h>
#include <CdeServices.h>

extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void abort(void);

/**
Synopsis
    #include <string.h>
    char *_strdup(const char *strSource);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strdup-wcsdup?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strdup-wcsdup-mbsdup?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strdup-wcsdup-mbsdup?view=msvc-160#return-value

**/
static char* _strdupCDEABI(const char* s1) 
{

    char* pRet = NULL;
    
    if (NULL != s1) do
    {
        pRet = malloc(strlen(s1));

        if (NULL == pRet)
            break;

        pRet = strcpy(pRet, s1);
    } while (0);

    return pRet;
}
MKCDEABI(_strdup);
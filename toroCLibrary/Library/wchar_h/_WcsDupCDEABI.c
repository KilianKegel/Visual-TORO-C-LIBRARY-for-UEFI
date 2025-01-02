/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _WcsDupCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Duplicates wide strings.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) wchar_t* wcscpy(wchar_t* pszDst, const wchar_t* pszSrc);
extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);
extern __declspec(dllimport) void* malloc(size_t size);

/**
Synopsis
    #include <string.h>
    wchar_t *_wcsdup(const wchar_t *strSource);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strdup-wcsdup?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strdup-wcsdup-mbsdup?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strdup-wcsdup-mbsdup?view=msvc-160#return-value

**/

static wchar_t* _wcsdupCDEABI(const wchar_t* s1) 
{
    wchar_t* pRet = NULL;
    
    if (NULL != s1) do
    {
        pRet = malloc(sizeof(wchar_t) * wcslen(s1));

        if (NULL == pRet)
            break;

        pRet = wcscpy(pRet, s1);
    } while (0);

    return pRet;
}

static wchar_t* wcsdupCDEABI(const wchar_t* s1)
{
    return _wcsdupCDEABI(s1);
}

MKCDEABI(wcsdup); 
MKCDEABI(_wcsdup);
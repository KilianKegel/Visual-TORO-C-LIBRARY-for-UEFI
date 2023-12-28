/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _WcsDup.c

Abstract:

    Implementation of the Standard C function.
    Duplicates wide strings.

Author:

    Kilian Kegel

--*/
#include <wchar.h>
#include <stdlib.h>

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
wchar_t* _wcsdup(const wchar_t* s1);

wchar_t* wcsdup(const wchar_t* s1) 
{
    return _wcsdup(s1);
}

wchar_t* _wcsdup(const wchar_t* s1) 
{
    wchar_t* pRet = malloc(sizeof(wchar_t) * wcslen(s1));
    
    return wcscpy(pRet, s1);
}


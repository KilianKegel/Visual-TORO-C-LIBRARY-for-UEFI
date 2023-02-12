/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _StrDup.c

Abstract:

    Implementation of the POSIX C function.
    Duplicates strings.

Author:

    Kilian Kegel

--*/
#include <string.h>
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
char* _strdup(const char* s1) 
{
    char* pRet = malloc(strlen(s1));
    
    return strcpy(pRet, s1);
}

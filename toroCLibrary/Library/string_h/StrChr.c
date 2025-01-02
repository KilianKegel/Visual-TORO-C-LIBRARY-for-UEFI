/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrChr.c

Abstract:

    Implementation of the Standard C function.
    Finds a character in a string, by using the current locale or a specified LC_CTYPE conversion-state category.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <string.h>

char* strpbrk(const char* pszStr, const char* pszSet);

/** strchr

Synopsis
    #include <string.h>
    char *strchr(const char *str, int c);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strchr-wcschr-mbschr-mbschr-l?view=msvc-160
    The strchr function locates the first occurrence of c (converted to a char) in the
    string pointed to by s. The terminating null character is considered to be part of the
    string.
Returns
    The strchr function returns a pointer to the located character, or a null pointer if the
    character does not occur in the string.


    @param[in] const char *str

    @param[in] int c

    @retval Success: pointer to the located charachter

    @retval Failure: NULL

**/
char* strchr(const char* str, int c) {

    char buffer[2] = { '\0','\0' };
    char* pRet;

    buffer[0] = (char)c;
    if ('\0' != c) {
        pRet = strpbrk(str, &buffer[0]);
    }
    else {
        pRet = &((char*)/*cast due "warning C4090: '=' : different 'const' qualifiers"*/str)[strlen(str)];
    }

    return pRet;
}
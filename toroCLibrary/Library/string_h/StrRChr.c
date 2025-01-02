/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrRChr.c

Abstract:

    Implementation of the Standard C function.
    Scans a string for the last occurrence of a character.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <string.h>
#include <stdio.h>

/** strchr
Synopsis
    #include <string.h>
    char *strrchr(const char *s, int c);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strrchr-wcsrchr-mbsrchr-mbsrchr-l?view=msvc-160
    The strrchr function  locates  the  last  occurrence  of c (converted  to  a
    char )in the string  pointed  to  by s.
    The  terminating  null  character  is  considered  to  be  part  of  the string.
Returns
    The strrchr function returns a pointer to the character, or a null pointer if c
    does not occur in the string.

    @param[in] const char *str

    @param[in] int c

    @retval Success: pointer to the located charachter

    @retval Failure: NULL

**/
char* strrchr(const char* str, int c) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char buffer[2] = { '\0','\0' };
    char* pRet;

    buffer[0] = (char)c;
    if ('\0' != c) {
        pRet = pCdeAppIf->pCdeServices->pWcsStrPbrkSpn(TDN, (void*)str, (void*)&buffer[0]);
    }
    else {
        pRet = &((char*)/*cast due "warning C4090: '=' : different 'const' qualifiers"*/str)[strlen(str)];
    }

    return pRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrPbrkCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Scans strings for characters in specified character sets

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void* __cdeGetAppIf(void);

/**

Synopsis
    #include <string.h>
    char *strpbrk(const char *s1, const char *s2);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strpbrk-wcspbrk-mbspbrk-mbspbrk-l?view=msvc-160
    The strpbrk function locates the first occurrence in the string pointed to by s1 of any
    character from the string pointed to by s2.
Returns
    The strpbrk function returns a pointer to the character, or a null pointer if
    no character from s2 occurs in s1.


    @param[in]  char *s1

    @param[in]  char *s2

    @retval size_t, length of block

**/
static char* strpbrkCDEABI(const char* pszStr, const char* pszSet) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char* p;

    p = (char*)pCdeAppIf->pCdeServices->pWcsStrPbrkSpn(0, (void*)pszStr, (void*)pszSet);

    return *p == '\0' ? NULL : p;
}

MKCDEABI(strpbrk);
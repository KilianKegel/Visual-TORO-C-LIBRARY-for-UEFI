/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrSpn.c

Abstract:

    Implementation of the Standard C function.
    Returns the index of the first character, in a string, that does not belong to a set of characters.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

extern void* __cdeGetAppIf(void);

/**

Synopsis
    #include <string.h>
    size_t strspn(const char *s1, const char *s2);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strspn-wcsspn-mbsspn-mbsspn-l?view=msvc-160
    The strspn function computes the length of the maximum initial segment of the string
    pointed to by s1 which consists entirely of characters from the string pointed to by s2.
Returns
    The strspn function returns the length of the segment.

    @param[in]  char *pszStr

    @param[in]  char *pszSet

    @retval size_t, length of block

**/

size_t strspn(const char* pszStr, const char* pszSet) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char* p;
    p = (char*)pCdeAppIf->pCdeServices->pWcsStrPbrkSpn(INV, (void*)pszStr, (void*)pszSet);
    return p ? p - pszStr : 0;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrCSpn.c

Abstract:

    Implementation of the Standard C function.
    Returns the index of the first occurrence in a string, of a character that belongs to a set of characters.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>
//
#define _TCHAR char

extern void* __cdeGetAppIf(void);

/**

Synopsis
    #include <string.h>
    size_t strcspn(const char *s1, const char *s2);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcspn-wcscspn-mbscspn-mbscspn-l?view=msvc-160
    The strcspn function computes the length of the maximum initial segment of the string
    pointed to by s1 which consists entirely of characters NOT from the string pointed to by s2.
Returns
    The strcspn function returns the length of the segment.

    @param[in]  char *pszStr

    @param[in]  char *pszSet

    @retval size_t, length of block

**/

size_t strcspn(const _TCHAR* pszStr, const _TCHAR* pszSet) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    _TCHAR* p;
    p = (_TCHAR*)pCdeAppIf->pCdeServices->pWcsStrPbrkSpn(0, (void*)pszStr, (void*)pszSet);
    return p ? p - pszStr : 0;
}
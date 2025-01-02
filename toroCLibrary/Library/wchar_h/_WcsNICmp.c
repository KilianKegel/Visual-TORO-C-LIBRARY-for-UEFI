/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _WcsNICmp.c

Abstract:

    Implementation of the Microsoft C function.
    Compares the specified number of characters of two strings without regard to case.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

static ROMPARM_MEMSTRXCMP ROMPARM = {
    /*fForceToDataSeg*/ 1 ,\
    /*fCountIsParm*/    1,  \
    /*fCaseSensitive*/  0,  \
    /*fBreakOnZero*/    1,  \
    /*fAjustDifference*/1,  \
    /*fWide*/           1 };


/**
Synopsis
    #include <wchar.h>
    int _wcsnicmp(const wchar_t* pszDst, const wchar_t* pszSrc, size_t count);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l?view=msvc-160#return-value

**/
int _wcsnicmp(const wchar_t* pszDst, const wchar_t* pszSrc, size_t count) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return pCdeAppIf->pCdeServices->pMemStrxCmp(&ROMPARM, (void*)pszDst, (void*)pszSrc, count);

}
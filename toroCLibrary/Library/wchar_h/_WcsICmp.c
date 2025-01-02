/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _WcsICmp.c

Abstract:

    Implementation of the ISO C++ function.
    Performs a case-insensitive comparison of wide strings.

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>

static ROMPARM_MEMSTRXCMP ROMPARM = {
    /*fForceToDataSeg*/ 1 ,\
    /*fCountIsParm*/    0,  \
    /*fCaseSensitive*/  0,  \
    /*fBreakOnZero*/    1,  \
    /*fAjustDifference*/1,  \
    /*fWide*/           1 };

/**
Synopsis
    #include <wchar.h>
    int _wcsicmp(const wchar_t* pszDst, const wchar_t* pszSrc);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stricmp-wcsicmp-mbsicmp-stricmp-l-wcsicmp-l-mbsicmp-l?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stricmp-wcsicmp-mbsicmp-stricmp-l-wcsicmp-l-mbsicmp-l?view=msvc-160
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stricmp-wcsicmp-mbsicmp-stricmp-l-wcsicmp-l-mbsicmp-l?view=msvc-160#return-value

**/
int _wcsicmp(const wchar_t* pszDst, const wchar_t* pszSrc) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return pCdeAppIf->pCdeServices->pMemStrxCmp(&ROMPARM, (void*)pszDst, (void*)pszSrc, (size_t)-1);

}
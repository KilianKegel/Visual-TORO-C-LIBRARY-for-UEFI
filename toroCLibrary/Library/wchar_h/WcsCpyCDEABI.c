/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    WcsCpyCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Copies a wide string. 

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**
Synopsis
    #include <wchar.h>
    wchar_t* wcscpy(wchar_t* pszDst, const wchar_t* pszSrc);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcpy-wcscpy-mbscpy?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcpy-wcscpy-mbscpy?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strcpy-wcscpy-mbscpy?view=msvc-160#return-value
**/
static wchar_t* wcscpyCDEABI(wchar_t* pszDst, const wchar_t* pszSrc) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return (wchar_t*)pCdeAppIf->pCdeServices->pMemStrxCpy(BOZ + WID, pszDst, pszSrc, (size_t)-1);

}

MKCDEABI(wcscpy);
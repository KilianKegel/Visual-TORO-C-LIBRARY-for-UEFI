/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wctombCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Convert a wide character to the corresponding multibyte character.

Author:

    Kilian Kegel

--*/
#define _INC_STDDEF         // exclude MSFT STDDEF.H, that conflicts with errno
#include <CdeServices.h>
#include <stddef.h>
//
// errno.h
//
#undef errno
extern __declspec(dllimport) int* _errno(void);
#define errno (*_errno())
#define EILSEQ 42

/**
Synopsis
    #include <stdlib.h>
    int wctomb(char* pmb, wchar_t wc);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wctomb-wctomb-l?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wctomb-wctomb-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wctomb-wctomb-l?view=msvc-160#return-value

**/
static int wctombCDEABI(char* pmb, wchar_t wc)
{
    int nRet = -1;
    do
    {
        if (NULL == pmb)
        {
            nRet = 0;
            break;
        }

        if (256 > wc)
        {
            *pmb = (char)wc;
            nRet = 1;
            break;
        }

        *pmb = '\0';
        errno = EILSEQ;// 42 BUGBUG errno documented by MSFT is 22;

    } while (0);

    return nRet;
}

MKCDEABI(wctomb);
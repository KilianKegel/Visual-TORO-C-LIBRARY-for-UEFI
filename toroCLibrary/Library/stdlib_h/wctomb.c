/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wctomb.c

Abstract:

    Implementation of the Standard C function.
    Convert a wide character to the corresponding multibyte character.

Author:

    Kilian Kegel

--*/
#include <stddef.h>
#include <errno.h>

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
int wctomb(char* pmb, wchar_t wc)
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
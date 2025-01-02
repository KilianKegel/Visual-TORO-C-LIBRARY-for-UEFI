/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wcstombs.c

Abstract:

    Implementation of the Standard C function.
    Converts a sequence of wide characters to a corresponding sequence of multibyte characters.

Author:

    Kilian Kegel

--*/
#include <stddef.h>

/**

Synopsis
    #include <stdlib.h>
    size_t wcstombs(char* mbstr, const wchar_t* wcstr, size_t count);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wcstombs-wcstombs-l?view=msvc-160
Paramaters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wcstombs-wcstombs-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wcstombs-wcstombs-l?view=msvc-160#return-value

**/
size_t wcstombs(char* mbstr, const wchar_t* wcstr, size_t count)
{
    size_t nRet = 0, i;
    wchar_t c = 0;

    for (i = 0; i < count; i++)
    {
        c = wcstr[i];

        if (255 < c)
            break;

        mbstr[i] = (char)c;

        if ('\0' == c)
            break;
        else
            nRet++;
    }

    return 255 < c ? -1 : nRet;
}
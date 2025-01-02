/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbstowcs.c

Abstract:

    Implementation of the Standard C function.
    Converts a sequence of multibyte characters to a corresponding sequence of wide characters.

Author:

    Kilian Kegel

--*/
#include <stddef.h>

/**

Synopsis
    #include <stdlib.h>
    size_t mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbstowcs-mbstowcs-l?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbstowcs-mbstowcs-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbstowcs-mbstowcs-l?view=msvc-160#return-value
**/
size_t mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count)
{
    size_t nRet = 0, i;

    for (i = 0; i < count; i++)
    {
        char c = mbstr[i];

        wcstr[i] = c;
        if ('\0' == c)
            break;
        else
            nRet++;
    }

    return nRet;
}
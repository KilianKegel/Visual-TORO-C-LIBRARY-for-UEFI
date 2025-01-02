/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wcsrtombs.c

Abstract:

    Convert a wide character string to its multibyte character string representation.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);
/**

Synopsis
    #include <wchar.h>
    size_t wcsrtombs(char* mbstr, const wchar_t** wcstr, size_t count, mbstate_t* mbstate);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/wcsrtombs
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=404
    @param[in]  *wcstr
                *mbstr
                count
                *mbstate - ignored

    @retval 1 size of converted character
    @retval 0 on failure


**/
static size_t wcsrtombsCDEABI(char* mbstr, const wchar_t** wcstr, size_t count, mbstate_t* mbstate)
{
    char c;
    size_t initial_count = count;
    wchar_t* pwc = (wchar_t*)*wcstr; 

    do {
        
        if (NULL != wcstr && 0 == wcslen(*wcstr))
        {
            initial_count = 0;
            break;
        }

        if (NULL == mbstr)
        {
            initial_count = NULL == wcstr ? 0 : wcslen(*wcstr);     // Microsoft decided to return remaining string length if NULL == destination, not my choice!!!
            break;
        }

        if (0 == count)
            break;

        c = (char)*pwc++;

        if(NULL != wcstr)
            *wcstr = (void*)pwc;

        *mbstr++ = c;

    } while ('\0' != c && --count);

    return initial_count;;
}


MKCDEABI(wcsrtombs);
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbrstowcs.c

Abstract:

    Converts a multibyte character string in the current locale to a corresponding 
    wide character string, with the capability of restarting in the middle of a 
    multibyte character.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

extern size_t strlen(const char* pszBuffer);
/**

Synopsis
    #include <wchar.h>
    size_t mbsrtowcs(wchar_t *wcstr, const char **mbstr, size_t count, mbstate_t *mbstate);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/mbsrtowcs
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=403
    @param[in]  *wcstr
                *mbstr
                count
                *mbstate - ignored

    @retval 1 size of converted character
    @retval 0 on failure


**/
size_t mbsrtowcs(wchar_t* wcstr, const char** mbstr, size_t count, mbstate_t* mbstate)
{
    wchar_t wc;
    size_t initial_count = count;
    char* pc = (char*)*mbstr; 

    do {
        
        if (NULL != mbstr && 0 == strlen(*mbstr))
        {
            initial_count = 0;
            break;
        }

        if (NULL == wcstr)
        {
            initial_count = NULL == mbstr ? 0 : strlen(*mbstr);     // Microsoft decided to return remaining string length if NULL == destination, not my choice!!!
            break;
        }

        if (0 == count)
            break;

        wc = (wchar_t)*pc++;
        
        if(NULL != mbstr)
            *mbstr = pc;

        *wcstr++ = wc;

    } while (L'\0' != wc && --count);

    return initial_count;;
}
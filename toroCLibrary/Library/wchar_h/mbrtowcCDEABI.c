/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbrtowcCDEABI.c

Abstract:

    Convert a multibyte character in the current locale into the
    equivalent wide character, with the capability of restarting
    in the middle of a multibyte character.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <wchar.h>
    size_t mbrtowc(wchar_t *wchar, const char *mbchar, size_t count, mbstate_t *mbstate);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/mbrtowc
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=401
    @param[in]  *wchar
                *mbchar
                count
                *mbstate - ignored

    @retval 1 size of converted character
    @retval 0 on failure


**/
static size_t mbrtowcCDEABI(wchar_t* wchar, const char* mbchar, size_t count, mbstate_t* mbstate)
{

    do
    {
        if (NULL == wchar || NULL == mbchar)
            break;

        *wchar = 0 != count ? *mbchar : L'\0';

    } while (0);

    return (NULL == mbchar || '\0' == *mbchar || 0 == count) ? 0 : 1;
}

MKCDEABI(mbrtowc);

// n = mbrtowc(NULL,    NULL,   0,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    NULL,   1,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    NULL,   2,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    NULL,   0,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    NULL,   1,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    NULL,   2,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "",     0,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "",     1,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "",     2,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "",     0,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "",     1,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "",     2,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "abc",  0,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "abc",  1,      NULL)           -> wc ''        0xFFFF, n 1
// n = mbrtowc(NULL,    "abc",  2,      NULL)           -> wc ''        0xFFFF, n 1
// n = mbrtowc(NULL,    "abc",  0,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(NULL,    "abc",  1,      &mbstate)       -> wc ''        0xFFFF, n 1
// n = mbrtowc(NULL,    "abc",  2,      &mbstate)       -> wc ''        0xFFFF, n 1
// n = mbrtowc(&wc,     NULL,   0,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(&wc,     NULL,   1,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(&wc,     NULL,   2,      NULL)           -> wc ''        0xFFFF, n 0
// n = mbrtowc(&wc,     NULL,   0,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(&wc,     NULL,   1,      &mbstate)       -> wc ''        0xFFFF, n 0
// n = mbrtowc(&wc,     NULL,   2,      &mbstate)       -> wc ''        0xFFFF, n 0
// 
// n = mbrtowc(&wc,     "",     0,      NULL)           -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "",     1,      NULL)           -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "",     2,      NULL)           -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "",     0,      &mbstate)       -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "",     1,      &mbstate)       -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "",     2,      &mbstate)       -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "abc",  0,      NULL)           -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "abc",  1,      NULL)           -> wc 'a'       0x0061, n 1
// n = mbrtowc(&wc,     "abc",  2,      NULL)           -> wc 'a'       0x0061, n 1
// n = mbrtowc(&wc,     "abc",  0,      &mbstate)       -> wc ' '       0x0000, n 0
// n = mbrtowc(&wc,     "abc",  1,      &mbstate)       -> wc 'a'       0x0061, n 1
// n = mbrtowc(&wc,     "abc",  2,      &mbstate)       -> wc 'a'       0x0061, n 1
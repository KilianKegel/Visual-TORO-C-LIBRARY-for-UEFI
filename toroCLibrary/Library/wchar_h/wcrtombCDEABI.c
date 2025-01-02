/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbrtowc.c

Abstract:

    Convert a wide character into its multibyte character representation.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <wchar.h>
    size_t wcrtomb(char *mbchar, wchar_t wchar, mbstate_t *mbstate);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/wcrtomb
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=402
    @param[in]  *mbchar
                *wchar
                *mbstate - ignored

    @retval 1 in any case


**/
static size_t wcrtombCDEABI(char* mbchar, wchar_t wchar, mbstate_t* mbstate)
{

    do
    {
        if (NULL == mbchar)
            break;

        *mbchar = (char)wchar;

        if(NULL != mbstate)
            mbstate->_Wchar = L'\0';

    } while (0);

    return 1;
}

MKCDEABI(wcrtomb);
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbrlenCDEABI.c

Abstract:

    Import Library version

    Determine the number of bytes that are required to complete a multibyte
    character in the current locale, with the capability of restarting in the 
    middle of a multibyte character.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

/**

Synopsis
    #include <wchar.h>
    size_t mbrlen(const char * str,size_t count,mbstate_t * mbstate);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/mbrlen
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=400
    @param[in]  str
                count
                mbstate

    @retval 1 on success
    @retval 0 on failure


**/
static size_t mbrlenCDEABI(const char* str, size_t count, mbstate_t* mbstate)
{
    return (NULL == str || 0 == count || NULL == mbstate) ? 0 : 1;
}

MKCDEABI(mbrlen);
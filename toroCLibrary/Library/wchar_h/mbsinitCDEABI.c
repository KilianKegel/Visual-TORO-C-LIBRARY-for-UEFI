/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mbsinit.c

Abstract:

    Tracks the state of a multibyte character conversion.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <wchar.h>
    int mbsinit(const mbstate_t* ps);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/mbsinit
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=399
    @param[in]  mbstate_t* ps

    @retval 1 on success
    @retval 0 on failure


**/
static int mbsinitCDEABI(const mbstate_t* ps)
{
    return ps == NULL || ps->_Wchar == 0;

}

MKCDEABI(mbsinit);

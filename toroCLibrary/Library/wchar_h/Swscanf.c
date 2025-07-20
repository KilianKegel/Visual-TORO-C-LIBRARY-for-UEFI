/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Swscanf.c

Abstract:

    Implementation of the Standard C function.
    Read formatted data from a wide string.

Author:

    Kilian Kegel

--*/

#include <wchar.h>
#include <stdarg.h>

/**

Synopsis
#include <wchar.h>
    int swscanf(const wchar_t * restrict s,
    const wchar_t * restrict format, ...);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-170
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=374
    The swscanf function is equivalent to fwscanf, except that the argument s specifies a
    wide string from which the input is to be obtained, rather than from a stream. Reaching
    the end of the wide string is equivalent to encountering end-of-file for the fwscanf function.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sscanf-sscanf-l-swscanf-swscanf-l?view=msvc-170#return-value
**/
int swscanf(wchar_t const* const  pwcsBuffer, wchar_t const* const  pwcsFormat, ...) {
    va_list ap;
    int nRet;

    va_start(ap, pwcsFormat);

    nRet = vswscanf(pwcsBuffer, pwcsFormat, ap);

    va_end(ap);

    return nRet;
}

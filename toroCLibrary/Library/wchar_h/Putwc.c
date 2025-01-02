/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Putwc.c

Abstract:

    Implementation of the Standard C function.
    Writes a wide character to a stream.

Author:

    Kilian Kegel

--*/
#include <stdio.h>

/** fputc
Synopsis
    #include <stdio.h>
    #include <wchar.h>
    wint_t putwc(wchar_t c, FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/putc-putwc?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=382
    The putwc function is equivalent to fputwc, except that if it is implemented as a
    macro, it may evaluate stream more than once, so that argument should never be an
    expression with side effects.
Returns
    The putwc function returns the wide character written, or WEOF.

**/
wint_t putwc(wchar_t c, FILE* stream) {
    return fputwc(c, stream);
}
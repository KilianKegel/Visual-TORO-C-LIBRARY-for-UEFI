/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fwide.c

Abstract:

    Implementation of the Standard C function.
    Unimplemented.

Author:

    Kilian Kegel

--*/
#include <stdio.h>

/** fwide
Synopsis
    #include <wchar.h>
    int fwide(FILE *stream,int mode);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwide?view=msvc-170#syntax
    NOTE: This function is NOT IMPLEMENTED by Microsoft
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=381
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwide?view=msvc-170#return-value
**/
int fwide(FILE* stream, int mode)
{
    return mode;
}
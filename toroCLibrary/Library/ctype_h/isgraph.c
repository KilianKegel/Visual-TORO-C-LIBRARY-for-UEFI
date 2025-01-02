/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isgraph.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isgraph(int c);
Description
    The isgraph function tests for any printing character except space (' ').

    The function returns non-zero if c is any character for which either
    isalnum or ispunct returns non-zero.

    @param[in] c character to test for isgraph

    @retval _ALPHA | ISDIGIT | ISPUNCT if matching letter

    @retval 0 if not

**/
int isgraph(int c) {

    return isalnum(c) | ispunct(c);

}

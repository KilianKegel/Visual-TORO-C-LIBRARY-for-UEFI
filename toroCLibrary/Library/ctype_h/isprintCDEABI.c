/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isprintCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// ctype.h
//
#define _BLANK 0x40

extern __declspec(dllimport) int isgraph(int c);

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isprint(int c);
Description
    The isprint function tests for any printing character including space (' ').

    The function returns non-zero if c is space or a character for which
    isgraph returns non-zero.

    @param[in] c character to test for isprint

    @retval _ALPHA | ISDIGIT | ISPUNCT if matching letter

    @retval 0 if not

**/
static int isprintCDEABI(int c) {

    return isgraph(c) | (c == '\x20' ? _BLANK : 0);

}

MKCDEABI(isprint);
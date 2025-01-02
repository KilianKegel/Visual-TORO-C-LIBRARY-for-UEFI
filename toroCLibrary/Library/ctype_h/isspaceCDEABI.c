/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isspaceCDEABI.c

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
#define _SPACE 0x08

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isspace(int c);
Description
    The isspace function tests for any character that is a standard white-space character or
    is one of a locale-specific set of characters for which isalnum is false. The standard
    white-space characters are the following: space (' '), form feed ('\f'), new-line
    ('\n'), carriage return ('\r'), horizontal tab ('\t'), and vertical tab ('\v'). In the
    "C" locale, isspace returns true only for the standard white-space characters.

    The function returns non-zero if c is any of:
    CR FF HT NL VT space
    or any other locale-specific space character.

    @param[in] c character to test for isprint

    @retval _ALPHA | ISDIGIT | ISPUNCT if matching letter

    @retval 0 if not

**/
static int isspaceCDEABI(int c) {

    return (c == 0x20) || (c == '\f') || (c == '\n') || (c == '\r') || (c == '\t') || (c == '\v') ? _SPACE : 0;

}

MKCDEABI(isspace);
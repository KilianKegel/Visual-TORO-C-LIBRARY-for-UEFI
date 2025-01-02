/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ispunct.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int ispunct(int c);
Description
    The ispunct function tests for any printing character that is one of a locale-specific set
    of punctuation characters for which neither isspace nor isalnum is true. In the "C"
    locale, ispunct returns true for every printing character for which neither isspace
    nor isalnum is true.

    The function returns non-zero if c is any of:
    ! " # % & ’ ( ) ; <
    = > ? [ \ ] * + , -
    . / : ^ _ { | } ~
    or any other implementation-defined punctuation character.

    In Windows 0x21 .. 0x2F, 0x3A .. 0x40, 0x5B .. 0x60, 0x7B .. 0x7E

    @param[in] c character to test for punct

    @retval ISPUNCT if punct letter

    @retval 0 if not

**/
int ispunct(int c) {

    return  ((c >= 0x21 && c <= 0x2F) ||
        (c >= 0x3A && c <= 0x40) ||
        (c >= 0x5B && c <= 0x60) ||
        (c >= 0x7B && c <= 0x7E)) ? _PUNCT : 0;
}


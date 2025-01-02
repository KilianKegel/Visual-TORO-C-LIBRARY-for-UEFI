/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isalnum.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>
/*
Synopsis
    include <ctype.h>
    int isalnum(int c);
Description
    The isalnum function tests for any character for which isalpha or isdigit is true.

    The function returns non-zero if c is any of:
    a b c d e f g h i j k l m n o p q r s t u v w x y z
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
    0 1 2 3 4 5 6 7 8 9
    or any other locale-specific alphabetic character.
Parameter

    @param[in] c : character to test

Return

    @return 0 : if no match
    @return !0: otherwise
*/
int isalnum(int c) {
    return isalpha(c) | isdigit(c);
}

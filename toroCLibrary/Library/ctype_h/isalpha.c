/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isalpha.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isalpha(int c);
Description
    The isalpha function tests for any character for which isupper or islower is true,
    or any character that is one of a locale-specific set of alphabetic characters for which
    none of iscntrl, isdigit, ispunct, or isspace is true.170) In the "C" locale,
    isalpha returns true only for the characters for which isupper or islower is true.

    The function returns non-zero if c is any of:
    a b c d e f g h i j k l m n o p q r s t u v w x y z
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
    or any other locale-specific alphabetic character.

    @param[in] c character to test for alpha

    @retval eighter ISLOWER or ISUPPER if upcase letter

    @retval 0 if not

**/
int isalpha(int c) {
    return isupper(c) | islower(c);
}

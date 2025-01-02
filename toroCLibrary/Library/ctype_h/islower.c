/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    islower.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int islower(int c);
Description
    The islower function tests for any character that is a lowercase letter or is one of a
    locale-specific set of characters for which none of iscntrl, isdigit, ispunct, or
    isspace is true. In the "C" locale, islower returns true only for the lowercase
    letters (as defined in 5.2.1).

    The function returns non-zero if c is any of:
    a b c d e f g h i j k l m n o p q r s t u v w x y z
    or any other locale-specific uppercase character.

    @param[in] c character to test for lowcase

    @retval ISLOWER if upcase letter

    @retval 0 if not

**/
int islower(int c) {
    return c >= 'a' && c <= 'z' ? _LOWER : 0;
}

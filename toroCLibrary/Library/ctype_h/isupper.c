/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isupper.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isupper(int c);
Description

    The isupper function tests for any character that is an uppercase letter or is one of a
    locale-specific set of characters for which none of iscntrl, isdigit, ispunct, or
    isspace is true. In the "C" locale, isupper returns true only for the uppercase
    letters (as defined in 5.2.1).

    The function returns non-zero if c is any of:
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
    or any other locale-specific uppercase character.

    @param[in] c character to test for upcase

    @retval ISUPPER if upcase letter

    @retval 0 if not

**/
int isupper(int c) {
    return c >= 'A' && c <= 'Z' ? _UPPER : 0;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    toupper.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int toupper(int c);
Description
    The toupper function converts a lowercase letter to a corresponding uppercase letter.
Returns
    If the argument is a character for which islower is true and there are one or more
    corresponding characters, as specified by the current locale, for which isupper is true,
    the toupper function returns one of the corresponding characters (always the same one
    for any giv en locale); otherwise, the argument is returned unchanged.

    The function returns the corresponding uppercase letter if one exists and
    if islower(c); otherwise, it returns c.

    @param[in] c character to test for upcase

    @retval ISUPPER if upcase letter

    @retval 0 if not

**/
int         /*@retval to upper case converted c*/
toupper     /*@decl*/
(int c      /*@parm c to be case converted*/)
{
    return islower(c) ? 'A' + c - 'a' : c;
}

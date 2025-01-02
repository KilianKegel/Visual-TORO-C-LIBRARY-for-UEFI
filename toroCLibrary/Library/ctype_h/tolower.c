/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    tolower.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/

#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int tolower(int c);
Description
    The tolower function converts an uppercase letter to a corresponding lowercase letter.
Returns
    If the argument is a character for which isupper is true and there are one or more
    corresponding characters, as specified by the current locale, for which islower is true,
    the tolower function returns one of the corresponding characters (always the same one
    for any giv en locale); otherwise, the argument is returned unchanged.

    The function returns the corresponding lowercase letter if one exists and
    if isupper(c); otherwise, it returns c.

    @param[in] c character to test for upcase

    @retval ISUPPER if upcase letter

    @retval 0 if not

**/
/*@retval to lower case converted c*/int tolower(int c/*@parm c to be converted to lower case*/)
{
    return isupper(c) ? 'a' + c - 'A' : c;
}

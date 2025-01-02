/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iscntrl.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <ctype.h>

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int iscntrl(int c);
Description
    The iscntrl function tests for any control character.

    The function returns non-zero if c is any of:
    BEL BS CR FF HT NL VT
    or any other implementation-defined control character.

    In Windows 0x00 .. 0x1F, 0x7F

    @param[in] c character to test for _CONTROL

    @retval _CONTROL if cntrl letter

    @retval 0 if not

**/
int iscntrl(int c) {

    return (c >= 0x00 && c <= 0x1F) || (c == 0x7F) ? _CONTROL : 0;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isxdigitCDEABI.c

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
#define _HEX 0x80

extern __declspec(dllimport) int isdigit(int c);

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isxdigit(int c);
Description
    The isxdigit function tests for any hexadecimal-digit character (as defined in 6.4.4.1).

    The function returns non-zero if c is any of:
    0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F

    @param[in] c character to test for xdigit

    @retval ISHEX if alnum letter

    @retval 0 if not

**/
static int isxdigitCDEABI(int c) {

    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || isdigit(c) ? _HEX : 0;
}

MKCDEABI(isxdigit);
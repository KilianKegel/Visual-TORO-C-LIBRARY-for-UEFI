/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    isdigitCDEABI.c

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
#define _DIGIT 0x04

/** Brief description of the function’s purpose.

Synopsis
    #include <ctype.h>
    int isdigit(int c);
Description
    The isdigit function tests for any decimal-digit character (as defined in 5.2.1).

    The function returns non-zero if c is any of:
    0 1 2 3 4 5 6 7 8 9

    @param[in] c character to test for DIGIT

    @retval ISDIGIT if upcase letter

    @retval 0 if not

**/
static int isdigitCDEABI(int c) {
    return c >= '0' && c <= '9' ? _DIGIT : 0;
}

MKCDEABI(isdigit);
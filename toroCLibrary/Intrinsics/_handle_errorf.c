/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _handle_errorf.c

Abstract:

    Microsoft specific helper function
    Adjusted for CDE usage.

Author:

    Kilian Kegel

--*/

void _handle_errorf(void) {
    volatile unsigned x = 0xDEADBEEF;

    while (0xDEADBEEF == x)
        ;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Abs.c

Abstract:

    Implementation of the Standard C function.
    Sends a signal to the executing program.

Author:

    Kilian Kegel

--*/
int abs(int i) {
    return i < 0 ? -i : i;
}

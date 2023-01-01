/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _gSTDOUTMode.c

Abstract:

    CDE internal:   1: force character size conversion to 8Bit for STDOUT and STDERR
                    0: leave UEFI Shell default 16Bit
                    ???

Author:

    Kilian Kegel

--*/
char _gSTDOUTMode = 0;   //0 == UEFI Shell default, 1 == ASCII only
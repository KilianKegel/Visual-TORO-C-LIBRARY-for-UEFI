/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __abort_behavior.c

Abstract:

    Microsoft specific intrinsic data

Author:

    Kilian Kegel

--*/
#define _WRITE_ABORT_MSG  0x1 // debug only, has no effect in release
#define _CALL_REPORTFAULT 0x2
char __abort_behavior = 2;

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __CheckForDebuggerJustMyCode.c

Abstract:

    Microsoft specific helper function
    https://docs.microsoft.com/en-us/cpp/build/reference/jmc?view=msvc-160
    Adjusted for CDE usage.

Author:

    Kilian Kegel

--*/
#include <stdio.h>

int __CheckForDebuggerJustMyCode(void)  // MSFT specific
{
#pragma warning (disable:4189/*local variable is initialized but not referenced*/)
    static char sayhello[] = { "FAKED: __CheckForDebuggerJustMyCode()" };
    volatile char *p = sayhello;

    return 0;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __chkstk.c

Abstract:

    Microsoft specific helper function
    https://docs.microsoft.com/en-us/windows/win32/devnotes/-win32-chkstk
    Adjusted for CDE usage.

Author:

    Kilian Kegel

--*/
#include <stdio.h>

void __chkstk(void) // MSFT specific
{
#pragma warning (disable:4189/*local variable is initialized but not referenced*/)
    static char sayhello[] = { "FAKED: __chkstk()" };
    volatile char *p = sayhello;

    return;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _set_printf_count_output.c

Abstract:

    Microsoft specific: enable %n in printf format string
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-printf-count-output?view=msvc-160
    
    NOTE: %n is enabled by default in "toro C Library" / CDE

Author:

    Kilian Kegel

--*/
int _set_printf_count_output(  int enable  )
{
    return 0;
}// MSFT specific

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreClock.c

Abstract:

    clock() for core mode (that is: CDE_APP_IF* pCdeAppIf is parameter)

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>

clock_t _cdeCoreClock(CDE_APP_IF* pCdeAppIf)
{
    unsigned long long cps = pCdeAppIf->pCdeServices->TSClocksPerSec / CLOCKS_PER_SEC;
    unsigned long long clk = (pCdeAppIf->pCdeServices->pGetTsc(pCdeAppIf) - pCdeAppIf->pCdeServices->TSClocksAtSystemStart);

    return (clock_t)(clk / cps);
}

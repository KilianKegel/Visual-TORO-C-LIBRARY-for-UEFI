/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    clock.c

Abstract:

    Implementation of the Standard C function.
    Calculates the wall-clock time used by the calling process.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <time.h>
#include <stdio.h>

/**

Synopsis
    #include <time.h>
    clock_t clock(void);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/clock?view=msvc-160
    The clock function determines the processor time used.
Returns
    The clock function  returns  the  implementation’s  best  approximation  to  the  processor
    time  used  by  the  program  since  the  beginning  of  an  implementation-defined  era  related
    only to the program invocation.  To determine the time in seconds, the value returned by
    the clock function should be divided by the value of the macro CLOCKS_PER_SEC.
    If the  processor  time  used  is  not  available  or  its  value  cannot  be  represented,  the  function
    returns the value (clock_t)(-1)

    @param[in] void

    @retval time_t, calendar time

**/
clock_t clock(void)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    unsigned long long cps = pCdeAppIf->pCdeServices->TSClocksPerSec / CLOCKS_PER_SEC;
    unsigned long long clk = (pCdeAppIf->pCdeServices->pGetTsc(pCdeAppIf) - pCdeAppIf->pCdeServices->TSClocksAtSystemStart);

    return (clock_t)(clk / cps);
}
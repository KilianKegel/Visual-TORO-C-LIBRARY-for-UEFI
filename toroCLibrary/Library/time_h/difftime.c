/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    difftime.c

Abstract:

    Implementation of the Standard C function.
    Finds the difference between two times.

Author:

    Kilian Kegel

--*/
#define difftime msft_difftime/*rename original difftime() to establish my own difftime()*/
#include <time.h>
#undef difftime

/** difftime() -- difference between two calendar times

Synopsis
    #include <time.h>
    double difftime(time_t time1, time_t time0);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/difftime-difftime32-difftime64?view=msvc-160
    The difftime function computes the difference between two calendar times:
    time1 - time0.
Returns
    The difftime function returns the difference expressed in seconds as a
    double.

    @param[in] time_t time1

    @param[in] time_t time0

    @retval double, time difference

**/
double _difftime64(time_t time1, time_t time0)
{
    return (double)(time1 - time0);
}

double difftime(time_t time1, time_t time0)
{
    return _difftime64(time1, time0);
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ctimeCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Convert a time value to a string and adjust for local time zone settings.

Author:

    Kilian Kegel

--*/
//#define ctime msft_ctime/*rename original ctime() to establish my own ctime()*/
//#include <time.h>
//#undef ctime
#include <CdeServices.h>

extern __declspec(dllimport) char* asctime(const /*struct tm*/void* timeptr);
extern __declspec(dllimport) /*struct tm*/void* localtime(const long long* timep);

/** ctime() -- time to string conversion

Synopsis
    #include <time.h>
    char *ctime(const time_t *timer);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ctime-ctime32-ctime64-wctime-wctime32-wctime64?view=msvc-160
    The ctime function converts the calendar time pointed to by timer to local time in the
    form of a string. It is equivalent to asctime(localtime(timer))
Returns
    The ctime function returns the pointer returned by the asctime function with that
    broken-down time as argument.

    @param[in] const time_t *timer

    @retval pointer to the string

**/
static char* _ctime64CDEABI(const /*time_t*/void* timer)
{
    return asctime(localtime(timer));
}

static char* ctimeCDEABI(const /*time_t*/void* timer)
{
    return _ctime64CDEABI(timer);
}

MKCDEABI(_ctime64);
MKCDEABI(ctime);
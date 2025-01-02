/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    asctime.c

Abstract:

    Implementation of the Standard C function.
    Convert a tm time structure to a character string.

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>
#include <time.h>

extern int snprintf(char* pszDest, size_t dwCount, const char* pszFormat, ...);
extern void* __cdeGetAppIf(void);
extern const char* wday_name_short[7];
extern const char* mon_name_short[12];

/**

Synopsis
    #include <time.h>
    char *asctime(const struct tm *timeptr);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/asctime-wasctime?view=msvc-160
    The asctime function  converts  the  broken-down  time  in  the  structure  pointed  to  by
    timeptr into a string in the form

    Sun Sep 16 01:03:52 1973\n\0

    using the equivalent of the following algorithm.

Returns

    The asctime function returns a pointer to the string.

    @param[in] const struct tm *timeptr

    @retval pointer to the string

*/
char* asctime(const struct tm* timeptr)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char* result = &pCdeAppIf->szAscTimeStaticInternal[0];

    snprintf(result, 27, "%.3s %.3s %2d %.2d:%.2d:%.2d %d\n",
        wday_name_short[timeptr->tm_wday],
        mon_name_short[timeptr->tm_mon],
        timeptr->tm_mday, timeptr->tm_hour,
        timeptr->tm_min, timeptr->tm_sec,
        1900 + timeptr->tm_year);
    return result;
}


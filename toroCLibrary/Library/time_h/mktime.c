/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mktime.c

Abstract:

    Implementation of the Standard C function.
    Convert the local time to a calendar value.

Author:

    Oskar Schirmer

--*/
#define mktime msft_mktime/*rename original mktime() to establish my own mktime()*/
#include <CdeServices.h>
#include <time.h>
#undef mktime

extern struct tm* _cdeGmltime(const long long* timep, struct tm* ptm);
extern signed long long _cdeGreg2Day(unsigned long long y, unsigned long long m, unsigned long long d);
extern void _cdeDay2Greg(long long r, unsigned long long* y, unsigned long long* m, unsigned long long* d);

/**
Synopsis
    #include <time.h>
    time_t mktime(struct tm* ptm);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mktime-mktime32-mktime64?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mktime-mktime32-mktime64?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mktime-mktime32-mktime64?view=msvc-160#return-value
**/
time_t _mktime64(struct tm* ptm)
{
    long long t;
    struct tm tm;
    if (ptm->tm_year < (1970 - 1900))
        return (long long)-1;
    t = (long long)_cdeGreg2Day(ptm->tm_year + 1900ULL, ptm->tm_mon + 1ULL, ptm->tm_mday);
    t = 24 * t + ptm->tm_hour;
    t = 60 * t + ptm->tm_min;
    t = 60 * t + ptm->tm_sec;
    if (t >= 0)
        *ptm = *_cdeGmltime(&t, &tm);
    else
        t = -1LL;
    return t;
}

time_t mktime(struct tm* ptm) {
    return _mktime64(ptm);
}
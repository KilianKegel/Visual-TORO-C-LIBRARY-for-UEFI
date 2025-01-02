/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    mktimeCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Convert the local time to a calendar value.

Author:

    Oskar Schirmer

--*/
#include <CdeServices.h>
typedef long long time_t;

struct tm
{
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_isdst; // daylight savings time flag
};

extern struct tm* _cdeGmltime(const long long* timep, /*struct tm*/void* ptm);
extern signed long long _cdeGreg2Day(unsigned long long y, unsigned long long m, unsigned long long d);

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
static time_t _mktime64CDEABI(struct tm* ptm)
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

static time_t mktimeCDEABI(/*struct tm*/void* ptm) {
    return _mktime64CDEABI(ptm);
}

MKCDEABI(_mktime64);
MKCDEABI(mktime);
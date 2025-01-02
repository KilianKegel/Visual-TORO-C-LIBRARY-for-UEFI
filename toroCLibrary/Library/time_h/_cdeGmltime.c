/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeGmltime.c

Abstract:

    Implementation of the _cdeGmltime() function
    Gregorian calendar computation
    convert from Gregorian calendar date representation to day count from 1970
    and vice versa. Year is 1582 <= y <= 9999, month is 1..12, day 1..31,
    date count epoch is 1/1/1970.

Author:

    Oskar Schirmer

--*/
#include <CdeServices.h>
#include <time.h>

signed long long _cdeGreg2Day(unsigned long long y, unsigned long long m, unsigned long long d);
void _cdeDay2Greg(long long r, unsigned long long* y, unsigned long long* m, unsigned long long* d);

struct tm* _cdeGmltime(const long long* timep, struct tm* ptm)
{
    struct tm* pTm = ptm;
    long long t = *timep;
    unsigned long long y, m, d;

    if (t < 0)
        return NULL;
    pTm->tm_sec = t % 60;
    t /= 60;
    pTm->tm_min = t % 60;
    t /= 60;
    pTm->tm_hour = t % 24;
    t /= 24;
    _cdeDay2Greg(t, &y, &m, &d);
    pTm->tm_year = (int)y - 1900;
    pTm->tm_mon = (int)m - 1;
    pTm->tm_mday = (int)d;
    pTm->tm_wday = (t + 4) % 7;
    pTm->tm_yday = (int)(t - _cdeGreg2Day(y, 1, 1));
    pTm->tm_isdst = 0;
    return pTm;;
}

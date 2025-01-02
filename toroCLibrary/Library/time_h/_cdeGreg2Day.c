/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeGreg2Day.c

Abstract:

    Implementation of the _cdeGreg2Day() function
    Gregorian calendar computation convert from Gregorian 
    calendar date representation to day count from 1970
    and vice versa. Year is 1582 <= y <= 9999, month is 1..12, 
    day 1..31, date count epoch is 1/1/1970.

Author:

    Oskar Schirmer

--*/
#include <CdeServices.h>
long long _cdeGreg2Day(unsigned long long y, unsigned long long m, unsigned long long d)
{
    signed long long r;
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    r = (1461 * y) / 4 - ((y / 100 + 1) * 3) / 4 + (153 * m + 3) / 5 + d - 719561;
    return r;
}

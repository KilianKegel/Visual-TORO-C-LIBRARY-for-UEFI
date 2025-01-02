/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeDay2Greg.c

Abstract:

    Implementation of the _day2greg() function
    Gregorian calendar computation
    convert from Gregorian calendar date representation to day count from 1970
    and vice versa. Year is 1582 <= y <= 9999, month is 1..12, day 1..31,
    date count epoch is 1/1/1970.

Author:

    Oskar Schirmer

--*/
#include <CdeServices.h>
#include <time.h>

void _cdeDay2Greg(long long r, unsigned long long* y, unsigned long long* m, unsigned long long* d)
{
    unsigned long long a, b, z;
    signed long long c;
    a = r + 719470;
    a = a * 400 / 146097 + 1;
    do {
        a -= 1;
        c = (1461 * a) / 4 - ((a / 100 + 1) * 3) / 4 - 719468;
    } while (c > r);
    c = r - c;
    b = (c * 5 + 2) / 153 + 3;
    z = c + 93 - (153 * b + 3) / 5;
    if (b > 12) {
        b -= 12;
        a += 1;
    }
    *y = a;
    *m = b;
    *d = z;
}

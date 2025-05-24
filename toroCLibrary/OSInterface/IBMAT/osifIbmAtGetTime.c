/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifIbmAtGetTime.c

Abstract:

    OS interface (osif) to get system time

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// time.h
//
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

extern __declspec(dllimport) long long mktime(struct tm* ptm);
extern struct tm* _cdeGmltime(const long long* timep, struct tm* ptm);


static int bcd2int(int bcd) {
    int h, l;

    h = 0x0F & (bcd / 16);
    l = 0x0F & (bcd);
    return h * 10 + l;

}

static int rtcrd8(unsigned char idx) {
    int nRet = 1;

        _cdeOUTByte(0x70, idx);
        nRet = 0xFF & _cdeINByte(0x71);

    return nRet;
}
static char inside(int v, int lo, int up) {
    return ((v >= lo) && (v <= up));
}

/** _osifGetTime() - get current time

Synopsis
    long long _osifIbmAtGetTime(IN CDE_APP_IF* pCdeAppIf);
Description
    get current time_t in seconds after 01/01/1970 00:00
Parameters
    CDE_APP_IF* pCdeAppIf
Returns
    time_t in seconds after 01/01/1970 00:00
**/
long long _osifIbmAtGetTime(IN CDE_APP_IF* pCdeAppIf) {

    struct tm tm, * ptm2;
    long long nRet = 0;
    struct tm* pTm = (struct tm*) & pCdeAppIf->StructTmLocalTimeStaticInternal[0];

    while (0x80 & rtcrd8(0x0A))                         // wait until end of update cycle
        ;
    tm.tm_sec = bcd2int(rtcrd8(0x00));                  // seconds after the minute (from 0)
    tm.tm_min = bcd2int(rtcrd8(0x02));                  // minutes after the hour (from 0)
    tm.tm_hour = bcd2int(rtcrd8(0x04));                 // hour of the day (from 0)
    tm.tm_mday = bcd2int(rtcrd8(0x07));                 // day of the month (from 1)
    tm.tm_mon = bcd2int(rtcrd8(0x08)) - 1;              // month of the year (from 0)
    tm.tm_year = bcd2int(rtcrd8(0x09));                 // years since 1900 (from 0)
    tm.tm_year += (100 * bcd2int(rtcrd8(0x32)) - 1900); // add century
    tm.tm_wday = 0;                                     // days since Sunday (from 0)
    tm.tm_yday = 0;                                     // day of the year (from 0)
    tm.tm_isdst = 0;                                    // Daylight Saving Time flag
    
    //
    // check, if date/time from RTC is valid, each element in range, otherwise init to 1.1.1970
    //
    if (!(inside(tm.tm_sec, 0, 61) &&
        inside(tm.tm_min, 0, 59) &&
        inside(tm.tm_hour, 0, 23) &&
        inside(tm.tm_mday, 1, 31) &&
        inside(tm.tm_mon, 0, 11) &&
        inside(tm.tm_year, 70, 270))) {
        unsigned i;
        char *pc = (char*)&tm;

        for (i = 0; i < sizeof(tm); i++)
            pc[i] = 0;
        tm.tm_mday = 1;
        tm.tm_year = 70;
    }

    nRet = mktime(&tm);

    ptm2 = _cdeGmltime(&nRet, pTm);

    return nRet;
}
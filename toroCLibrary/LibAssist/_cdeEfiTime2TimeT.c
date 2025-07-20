/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeEfiTime2TimeT.c

Abstract:

    Convert EFI_TIME to 64Bit time_t

Author:

    Kilian Kegel

--*/
#include <uefi.h>
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
typedef long long time_t;// don't use _CRT_NO_TIME_T

/** _cdeEfiTime2TimeT()

Synopsis 

    time_t _cdeEfiTime2TimeT(EFI_TIME* pEfiTime)

Description

    Convert EFI_TIME to 64Bit time_t

Returns

    @param[in] EFI_TIME* pEfiTime

    @retval time_t

**/
time_t _cdeEfiTime2TimeT(EFI_TIME* pEfiTime)
{
    //
    // fill "struct tm"-compatible fields
    //
    struct tm timetm;

    timetm.tm_sec = pEfiTime->Second;
    timetm.tm_min = pEfiTime->Minute;
    timetm.tm_hour = pEfiTime->Hour;
    timetm.tm_mday = pEfiTime->Day;
    timetm.tm_mon = pEfiTime->Month - 1;
    timetm.tm_year = pEfiTime->Year - 1900;
    timetm.tm_wday = 0;
    timetm.tm_yday = 0;
    timetm.tm_isdst = 0;

    return mktime(&timetm);
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTGetTime.c

Abstract:

    OS interface (osif) to get system time WIndows NT

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <time.h>

/** _osifGetTime() - get current time

Synopsis
    INT64 _osifWinNTGetTime(IN void* pCdeAppIf);
Description
    get current time_t in seconds after 01/01/1970 00:00
Parameters
    CDE_APP_IF* pCdeAppIf
Returns
    time_t in seconds after 01/01/1970 00:00
**/
INT64 _osifWinNTGetTime(IN void/*CDE_APP_IF*/* pCdeAppIf) {

    struct tm tm, * ptm2;
    INT64 nRet;
    SYSTEMTIME st, * pst = &st;


    GetSystemTime(pst);

    tm.tm_sec = pst->wSecond;                           // seconds after the minute (from 0)
    tm.tm_min = pst->wMinute;                           // minutes after the hour (from 0)
    tm.tm_hour = pst->wHour;                            // hour of the day (from 0)
    tm.tm_mday = pst->wDay;                             // day of the month (from 1)
    tm.tm_mon = pst->wMonth - 1;                        // month of the year (from 0)
    tm.tm_year = pst->wYear - 1900;                     // years since 1900 (from 0)
    tm.tm_wday = 0;                                     // days since Sunday (from 0)
    tm.tm_yday = 0;                                     // day of the year (from 0)
    tm.tm_isdst = 0;                                    // Daylight Saving Time flag

    //printf("%s(%d): %04d/%02d/%02d, %02d:%02d:%02d\n",__FILE__,__LINE__,1900 + tm.tm_year,1 + tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    nRet = mktime(&tm);
    //printf("%s(%d): %04d/%02d/%02d, %02d:%02d:%02d\n",__FILE__,__LINE__,1900 + tm.tm_year,1 + tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);

    ptm2 = localtime(&nRet);
    //printf("%s(%d): %04d/%02d/%02d, %02d:%02d:%02d\n",__FILE__,__LINE__,1900 + ptm2->tm_year,1 + ptm2->tm_mon,ptm2->tm_mday,ptm2->tm_hour,ptm2->tm_min,ptm2->tm_sec);

    return nRet;
}

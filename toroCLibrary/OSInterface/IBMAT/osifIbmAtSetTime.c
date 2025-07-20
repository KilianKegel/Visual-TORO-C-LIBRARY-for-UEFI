/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifIbmAtSetTime.c

Abstract:

    OS interface (osif) to set system time
    NYI not yet implemented

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <time.h>

int printf(const char* pszFormat, ...);

/** SetTime

    SetTime() returns the current time of the system in seconds
    since epoch.

    @param[in] long long seconds since 1.1.1970

    @retval seconds since 1.1.1970

**/

int bcd2int(int bcd) {
    int h, l;
    h = 0x0F & (bcd / 16);
    l = 0x0F & (bcd);
    return h * 10 + l;
}

int rtcrd8(int idx) {
    int nRet = 1;

    /*/    _cdeOUTByte(0x70,idx);
            nRet = 0xFF & _cdeINByte(0x71);
    /**/
    return nRet;
}

long long _osifIbmAtSetTime(IN CDE_APP_IF* pCdeAppIf, long long qwTime) {

    return 0;
}

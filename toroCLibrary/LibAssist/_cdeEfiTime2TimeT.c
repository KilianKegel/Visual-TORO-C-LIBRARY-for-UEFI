/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeEfiTime2TimeT.c

Abstract:

    Convert EFI_TIME to 64Bit time_t

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <time.h>

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
    struct tm timetm = {
        .tm_sec = pEfiTime->Second,
        .tm_min = pEfiTime->Minute,
        .tm_hour = pEfiTime->Hour,
        .tm_mday = pEfiTime->Day,
        .tm_mon = pEfiTime->Month,
        .tm_year = pEfiTime->Year - 1900,
        .tm_wday = 0,
        .tm_yday = 0,
        .tm_isdst = 0
    };

    return mktime(&timetm);
}
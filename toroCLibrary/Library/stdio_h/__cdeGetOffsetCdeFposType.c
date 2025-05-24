/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeOffsetCdeFposType.c

Abstract:

    Toro C Library internal helperfunction that extracts 
    true "long long" OFFSET from CDEFPOS_T / fpos_t
    NOTE: Function returns the full 64bit signed offset
            for all cases:
            1. CDEFPOS_T
            2. native fpos_t

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdio.h>

extern int __cdeIsCdeFposType(long long fpos);
/**

Synopsis

    fpos_t __cdeOffsetCdeFposType(fpos_t fpos);

Description
    
    Toro C Library internal helperfunction that extracts
    true "long long" OFFSET from CDEFPOS_T / fpos_t
    NOTE: Function returns the full 64bit signed offset
            for all cases:
            1. CDEFPOS_T
            2. native fpos_t

           Bitnum 
            666 6
            321 0
        --------------------------
            000 x   CDE_SEEK_BIAS-less positive offset
            100 S   CDE_SEEK_BIAS           4 & 11b -> SEEK_SET
            101 S   CDE_SEEK_BIAS_APPEND    5 & 11b -> SEEK_CUR unused in CDE, instead it is APPEND
            110 S   CDE_SEEK_BIAS           6 & 11b -> SEEK_END
            111 x   CDE_SEEK_BIAS-less negative offset

Parameters

    long long fpos  :   file position

Returns
    long long offset
**/
fpos_t __cdeOffsetCdeFposType(fpos_t fpos)
{
    CDEFPOS_T CdeFPos = { .fpos64 = fpos };

    if (__cdeIsCdeFposType(fpos))                       // if CdeFpos_Type:
        if (1 == CdeFPos.CdeFposBias.Sign)              // if negative number == sign bit set...
            CdeFPos.CdeFposBias.Bias = 7;               // ...copy the sign == 1 from bit 60 to bits 61..63
        else
            CdeFPos.CdeFposBias.Bias = 0;               // ...copy the sign == 0 from bit 60 to bits 61..63

    return CdeFPos.fpos64;
}


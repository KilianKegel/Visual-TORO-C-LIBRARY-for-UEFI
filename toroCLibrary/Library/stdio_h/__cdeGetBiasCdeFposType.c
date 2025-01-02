/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeBiasCdeFposType.c

Abstract:

    Toro C Library internal helperfunction that extracts 
    bias/SEEK_SET/SEEK_CUR_SEEK_END value from CDEFPOS_T
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

    int __cdeBiasCdeFposType(long long fpos);

Description
    
    Toro C Library internal helperfunction that extracts
    bias -> SEEK_SET/SEEK_CUR/SEEK_END value from CDEFPOS_T

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

    fpos_t fpos  :   file position

Returns
    bias    :   SEEK_SET    0
                SEEK_END    2
                SEEK_APPEND 1   this is CDE specific
**/
int __cdeBiasCdeFposType(fpos_t fpos)
{
    CDEFPOS_T CdeFPos = { .fpos64 = fpos };
    int bias = SEEK_SET;                                            // normal case for non-CdeFPos type fpos_t

    if (__cdeIsCdeFposType(fpos))
        bias = (int)CdeFPos.CdeFposBias.Bias & CDE_SEEK_BIAS_MSK;   // clear highest bit

    return bias;
}


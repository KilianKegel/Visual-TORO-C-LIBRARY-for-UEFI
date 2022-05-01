/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeIsCdeFposType.c

Abstract:

    Toro C Library internal helperfunction determines if a fpos_t
    is a CDEFPOS_T or a native fpos_t

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdio.h>

/**

Synopsis

    int __cdeIsCdeFposType(long long fpos);

Description
    
    Toro C Library internal helperfunction determines if a fpos_t
    is a CDEFPOS_T or a native fpos_t

           Bitnum
            666 6
            321 0
        --------------------------
            000 x	CDE_SEEK_BIAS-less positive offset
            100 S	CDE_SEEK_BIAS	-4 -> SEEK_SET

            110 S	CDE_SEEK_BIAS	-4 -> SEEK_END
            111 x	CDE_SEEK_BIAS-less negative offset

Parameters

    long long fpos  :   file position

Returns
    0           on FAILURE (non-CdeFposType)
    1           on SUCCESS (CdeFposType)
**/
int __cdeIsCdeFposType(fpos_t fpos)
{
    CDEFPOS_T CdeFPos = { .reg64 = fpos };

    return CDE_SEEK_BIAS_SET == CdeFPos.CdeFposBias.Bias
        /*|| CDE_SEEK_BIAS_CUR == CdeFPos.CdeFposBias.Bias*/
        || CDE_SEEK_BIAS_END == CdeFPos.CdeFposBias.Bias;

}


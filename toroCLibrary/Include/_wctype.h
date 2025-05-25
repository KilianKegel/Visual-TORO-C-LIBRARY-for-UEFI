/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _wctype.h

Abstract:

    Definition of CDE internal structs to deal with WCTYPE.H

Author:

    Kilian Kegel

--*/
#ifndef __WCTYPE_H_
#define __WCTYPE_H_

typedef unsigned short wint_t;

typedef struct tagISWX_TRUTH_TABLE
{
    //NOTE: word0 <  word1 -> range
    //      word0 >  word1 -> 2 single word values
    //      word0 == word1 -> list end, word0 valid if !=0xFFFF
    unsigned short word0;
    unsigned short word1;
}ISWX_TRUTH_TABLE;

typedef struct _WCPROPERTY {
    wint_t type;
    int(*fun)(wint_t wc);
    const char *szProperty;
}WCPROPERTY;

#endif//__WCTYPE_H_

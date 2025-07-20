/*
++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeXAM.c

Abstract:

    CDE internal EXAMINE floating point number.

Author:

    Kilian Kegel

--*/            
#include <CdeServices.h>
extern unsigned short __cde80387FXAM(double x);
//
//  C3 C2 C0 C1 Meaning
//
//  0  0  0  0  +unsupported
//  0  0  0  1  -unsupported
//  0  0  1  0  +NAN
//  0  0  1  1  -NAN
//  0  1  0  0  +normal
//  0  1  0  1  -normal
//  0  1  1  0  +infinity
//  0  1  1  1  -infinity
//  1  0  0  0  +0
//  1  0  0  1  -0
//  1  0  1  0  +empty
//  1  0  1  1  -empty
//  1  1  0  0  +denormal
//  1  1  0  1  -denormal

//
//                 80387 STATUS WORD 
//
//   15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  +-----------------------------------------------+
//  |B |C3|   ST   |C2|C1|C0|IR|xx|PE|UE|OE|ZE|DE|IE|
//  +-----------------------------------------------+

int __cdeFXAM(double x) 
{
    unsigned short status = __cde80387FXAM(x);
    char sign = 0x80 * (0 != (status & 0x0200));    // check C1 sign bit
    int nRet = 0;

    //
    // NAME B  C3  --ST-- C2 C1 C0  Meaning
    // POS  B  6  5  4  3  2  1  0
    //      x  0  x  x  x  0  0  0   +unsupported  0
    //      x  0  x  x  x  0  1  0   -unsupported  0
    //      x  0  x  x  x  0  0  1   +NAN          1
    //      x  0  x  x  x  0  1  1   -NAN          1
    //      x  0  x  x  x  1  0  0   +normal       4
    //      x  0  x  x  x  1  1  0   -normal       4
    //      x  0  x  x  x  1  0  1   +infinity     5
    //      x  0  x  x  x  1  1  1   -infinity     5
    //      x  1  x  x  x  0  0  0   +0            40
    //      x  1  x  x  x  0  1  0   -0            40
    //      x  1  x  x  x  0  0  1   +empty        41
    //      x  1  x  x  x  0  1  1   -empty        41
    //      x  1  x  x  x  1  0  0   +denormal     44
    //      x  1  x  x  x  1  1  0   -denormal     44

    typedef enum {
        SIGN = 1,
        ZERO = 2,
        NORMAL = 4,
        DENORMAL = 8,
        INF = 16,
        NAN = 32,
        EMPTY = 64,
        UNSUPPORTED = 128,
        UNKNOWN = 256
    }CDEXAMDOUBLE_T;//examine

    switch (0x4700 & status)
    {
        case 0x0000: 
            nRet = UNSUPPORTED; // +unsupported
            break;
        case 0x0001:
            nRet = NAN;
            break;
        case 0x0004:
            nRet = NORMAL;
            break;
        case 0x0005:
            nRet = INF;
            break;
        case 0x0040:
            nRet = ZERO;
            break;
        case 0x0041:
            nRet = EMPTY;
            break;
        case 0x0044:
            nRet = DENORMAL;
            break;
        default:
            nRet = UNKNOWN;
            break;
    }
    return nRet | sign;
}



//  C3 C2 C1 C0  Meaning
//
//  0  0  0  0   +unsupported
//  0  0  1  0   -unsupported
//  0  0  0  1   +NAN
//  0  0  1  1   -NAN
//  0  1  0  0   +normal
//  0  1  1  0   -normal
//  0  1  0  1   +infinity
//  0  1  1  1   -infinity
//  1  0  0  0   +0
//  1  0  1  0   -0
//  1  0  0  1   +empty
//  1  0  1  1   -empty
//  1  1  0  0   +denormal
//  1  1  1  0   -denormal


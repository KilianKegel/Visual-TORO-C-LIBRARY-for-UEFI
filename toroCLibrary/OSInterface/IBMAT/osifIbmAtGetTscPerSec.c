/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifIbmAtGetTscPerSec.c

Abstract:

    OS interface (osif) to get TSC per second

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <intrin.h>

extern size_t __cdeGetEFLAGS(void);

#define TIMER 2

/** _osifIbmAtGetTscPerSec - calulates TSC clocks per second

    GetTscPerSec() returns the TimeStampCounter counts per second

    NTSC Color Subcarrier:  f = 3.579545MHz * 4 ->
                            f = 14.31818MHz / 12 -> 1.193181666...MHz
    PIT 8254 input clk:     f = 1.193181666MHz
                            f = 11931816666Hz / 59659 ->
                            f = 20Hz -> t = 1/f
                            t = 50ms
                            ========
                            50ms * 20 ->

                                  1s
                            ===============

    @param[in] VOID

    @retval number of CPU clock per second

**/
unsigned long long _osifIbmAtGetTscPerSec(IN CDE_APP_IF* pCdeAppIf) {

#define CLKWAIT (59659)
    size_t eflags = __cdeGetEFLAGS();            // save flaags
    unsigned long long qwTSCEnd, qwTSCStart, qwTSCdiff, qwTSCPerIntervall;
    unsigned char bCountLo, bCountHi;
    unsigned short wCount;
    unsigned short wTicksGoneThrough;

    __CDEINTERRUPT_DISABLE;

    _cdeOUTByte(0x61, 0);                          // stop counter
    _cdeOUTByte(0x43, (TIMER << 6) + 0x34);        // program timer 2 for MODE 2
    _cdeOUTByte(0x42, 0xFF);                       // write counter value low 65535
    _cdeOUTByte(0x42, 0xFF);                       // write counter value high 65535
    _cdeOUTByte(0x61, 1);                          // start counter

    qwTSCStart = __rdtsc();                 // get TSC start

    //
    // repeat counter latch command until 50ms
    //
    do                                                              //
    {                                                               //
        _cdeOUTByte(0x43, (TIMER << 6) + 0x0);                      // counter latch timer 2
                                                                    //
        bCountLo = (unsigned char)_cdeINByte(0x40 + TIMER);           // get low v
        bCountHi = (unsigned char)_cdeINByte(0x40 + TIMER);           // get high v
                                                                    //
        wCount = (bCountHi << 8) | bCountLo;                        //
                                                                    //
    } while (wCount > (65535 - CLKWAIT));                           // until 59659 ticks gone

    qwTSCEnd = __rdtsc();                                           // get TSC end ~50ms

    qwTSCdiff = qwTSCEnd - qwTSCStart;

    wCount = 65535 - wCount;                                        // get true, not inverted, number of clock ticks...
                                                                    // ... that really happened
    wTicksGoneThrough = wCount - CLKWAIT;                           // get the number of additional ticks gone through

    //
    // error correction
    //
    //            qwTSCdiff               qwTSCPerIntervall
    //     -------------------------- == -------------------     -->
    //     CLKWAIT + wTicksGoneThrough         CLKWAIT
    //
    //
    //                             qwTSCdiff * CLKWAIT
    //      qwTSCPerIntervall = --------------------------
    //                          CLKWAIT + wTicksGoneThrough
    //
    
    //
    // calculate number of CPU TSC per requested intervall CLKWAIT (59659) i8254 timer ticks
    //
    qwTSCPerIntervall = (qwTSCdiff * CLKWAIT) / (CLKWAIT + (unsigned long long)wTicksGoneThrough);

    if (0x200 & eflags)                                 // restore IF interrupt flag
        __CDEINTERRUPT_ENABLE;

    return 20 * qwTSCPerIntervall;                      // subtract the drift from TSC difference, scale to 1 second
}
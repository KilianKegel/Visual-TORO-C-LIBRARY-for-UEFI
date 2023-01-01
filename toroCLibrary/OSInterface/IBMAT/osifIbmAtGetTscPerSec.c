/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifIbmAtGetTscPerSec.c

Abstract:

    OS interface (osif) to get TSC per second

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void _disable(void);
extern void _enable(void);

#pragma intrinsic (_disable, _enable)

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

    size_t eflags = __readeflags();         // save flaags
    unsigned long long qwTSCPerTick, qwTSCEnd, qwTSCStart, qwTSCDrift;
    unsigned char counterLoHi[2];
    unsigned short* pwCount = (unsigned short*)&counterLoHi[0];
    unsigned short wCountDrift;

    _disable();

    outp(0x61, 0);                          // stop counter
    outp(0x43, (TIMER << 6) + 0x34);        // program timer 2 for MODE 2
    outp(0x42, 0xFF);                       // write counter value low 65535
    outp(0x42, 0xFF);                       // write counter value high 65535
    outp(0x61, 1);                          // start counter

    qwTSCStart = __rdtsc();                 // get TSC start

    //
    // repeat counter latch command until 50ms
    //
    do                                                              //
    {                                                               //
        outp(0x43, (TIMER << 6) + 0x0);                             // counter latch timer 2
        counterLoHi[0] = (unsigned char)inp(0x40 + TIMER);          // get low byte
        counterLoHi[1] = (unsigned char)inp(0x40 + TIMER);          // get high byte
                                                                    //
    } while (*pwCount > (65535 - 59659));                           // until 59659 ticks gone

    qwTSCEnd = __rdtsc();                               // get TSC end ~50ms

    *pwCount = 65535 - *pwCount;                        // get true, not inverted, number of clock ticks...
                                                        // ... that really happened
    wCountDrift = *pwCount - 59659;                     // get the number of additional ticks gone through

    //
    // approximate the additional number of TSC
    //
    qwTSCPerTick = (qwTSCEnd - qwTSCStart) / *pwCount;  // get number of CPU TSC per 8254 ClkTick (1,19MHz)
    qwTSCDrift = wCountDrift * qwTSCPerTick;            // get TSC drift

    //printf("PITticks gone: %04d, PITticksDrift: %04d, TSCPerTick: %lld, TSCDrift: %5lld, TSC end/start diff: %lld, end - start - TSCDrift: %lld, TSC/Sec: %lld\n",
    //    *pwCount,                                       /* PITticks gone            */
    //    wCountDrift,                                    /* PITticksDrift            */
    //    qwTSCPerTick,                                   /* TSCPerTick               */
    //    qwTSCDrift,                                     /* TSCDrift                 */
    //    qwTSCEnd - qwTSCStart,                          /* TSC end/start diff       */
    //    qwTSCEnd - qwTSCDrift - qwTSCStart,             /* end - start - TSCDrift   */
    //    20 * (qwTSCEnd - qwTSCStart - qwTSCDrift)       /* TSC/Sec                  */
    //);

    if (0x200 & eflags)                                 // restore IF interrupt flag
        _enable();

    return 20 * (qwTSCEnd - qwTSCStart - qwTSCDrift);   // subtract the drift from TSC difference, scale to 1 second
}
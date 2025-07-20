/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiShellGetTscPerSec.c

Abstract:

    OS interface (osif) to get PerformanceCounter frequency using ACPI PmTmr

Author:

    Kilian Kegel

--*/
#define OS_EFI
//#define _INC_STDDEF           // exclude MSFT STDDEF.H, that conflicts with errno
#include <CdeServices.h>
#include <cde.h>

#include <stdint.h>
#include <intrin.h>

/** _osifIbmAtGetTscPerSec - calulates TSC clocks per second

    GetTscPerSec() returns the TimeStampCounter counts per second

    https://uefi.org/sites/default/files/resources/ACPI_Spec_6_4_Jan22.pdf#page=130

    ACPI timer frequency:   f = 3.579545MHz
                            
                            f = 3.579545MHz / (62799 * 3) -> 19.00001302858036245510568350104 Hz
                            f = 19Hz -> t = 1/f
                            t = 52.63154285714285714285714285714 ms
                            ========
                            52.63 ms * 19 ->

                                  1s
                            ===============

    @param[in] VOID

    @retval number of CPU clock per second

**/
static uint16_t GetACPICount(uint16_t AcpiPmTmrBase)
{
    return UINT16_MAX & _cdeINDWord(AcpiPmTmrBase);                // NOTE: PmTmr requires 32Bit read!
}

UINT64 _osifUefiShellGetTscPerSec(IN CDE_APP_IF* pCdeAppIf, uint16_t AcpiPmTmrBase) 
{

#define CLKWAIT (62799 * 3)
    int64_t  count = CLKWAIT;
    uint64_t qwTSCPerIntervall = 100000, qwTSCEnd, qwTSCStart, qwTicksGoneThrough;
    size_t eflags = __readeflags();                     // save flaags
    
    //AcpiPmTmrBase = 0x408;//kgtest
    
    __CDEINTERRUPT_DISABLE;
    GetACPICount(AcpiPmTmrBase);
    
    if (1)
    {
        uint16_t previous, current, diff = 0;

        previous = GetACPICount(AcpiPmTmrBase);
        qwTSCStart = __rdtsc();                             // get TSC start

        while (count > 0)
        {
            current = GetACPICount(AcpiPmTmrBase);

            if (current >= previous)
                diff = current - previous;
            else
                diff = ~(previous - current) + 1;

            previous = current;

            count -= diff;
        }

        qwTSCEnd = __rdtsc();                               // get TSC end ~50ms
        
        qwTicksGoneThrough = qwTSCEnd - qwTSCStart;           // calculate the additional number of TSC

        //
        //     qwTicksGoneThrough     qwTSCPerIntervall
        //     ------------------ == -------------------      -->
        //      CLKWAIT - count          CLKWAIT
        //
        //
        //                           qwTicksGoneThrough * CLKWAIT
        //      qwTSCPerIntervall = -----------------------------
        //                               CLKWAIT - count
        //
        //          NOTE: "count" is negative. " - count " ADDs additional ticks gone through
        //
        qwTSCPerIntervall = ((qwTicksGoneThrough) * CLKWAIT) / (CLKWAIT - count);    // get number of CPU TSC per ACPI PmTmr ClkTick (3.57MHz)

        if (0x200 & eflags)                                 // restore IF interrupt flag
            __CDEINTERRUPT_ENABLE;
    }

    return (int64_t) 19 * qwTSCPerIntervall;
}

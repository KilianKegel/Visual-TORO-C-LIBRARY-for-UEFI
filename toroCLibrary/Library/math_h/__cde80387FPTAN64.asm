;/*++
;
;    toro C Library
;    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
;
;    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
;    SPDX-License-Identifier: GNU General Public License v3.0
;
;Module Name:
;
;    __cde80387FPTAN64.asm
;
;Abstract:
;
;    CDE internal math 80387 instruction FPTAN to calculate tangens.
;
;       double __cde80387FPTAN(double x)
;
;Author:
;
;    Kilian Kegel
;
;--*/
.code
__cde80387FPTAN proc

    local float64:QWORD

    movsd float64,xmm0

    fld float64

    FPTAN
    FSTP    ST(0)

    fstp float64

    movsd xmm0,float64

    ret

__cde80387FPTAN endp
end
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
;    __cde80387FPTAN32.asm
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
    .model flat
.code
__cde80387FPTAN proc C public float64:QWORD

    fld float64

    FPTAN
    FSTP    ST(0)

    ret

__cde80387FPTAN endp
end
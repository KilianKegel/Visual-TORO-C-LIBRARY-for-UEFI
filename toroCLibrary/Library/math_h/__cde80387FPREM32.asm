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
;    __cde80387FPREM32.asm
;
;Abstract:
;
;   CDE internal math 80387 instruction FPREM to calculate floating point remainder.
;
;       double __cde80387FPREM(double x, double y)
;
;Author:
;
;    Kilian Kegel
;
;--*/
    .model flat
.code
__cde80387FPREM proc C public float64X:QWORD, float64Y:QWORD

    fld float64Y
    fld float64X

L1: FPREM
    FSTSW AX
    test AH, 4h ; test C2
    jnz L1

    fstp    st(1)

    ret

__cde80387FPREM endp
end
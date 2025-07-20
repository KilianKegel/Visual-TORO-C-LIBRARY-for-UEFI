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
;    __cde80387FYL2X64.asm
;
;Abstract:
;
;   CDE internal math 80387 instruction FYL2X to calculate y * ld(x).
;
;       double __cde80387FYL2X(double x, double y)
;
;Author:
;
;    Kilian Kegel
;
;--*/
.code
__cde80387FYL2X proc

    local float64X:QWORD
    local float64Y:QWORD

    movsd float64X,xmm0
    movsd float64Y,xmm1

    fld float64Y
    fld float64X

    FYL2X

    fstp float64X

    movsd xmm0,float64X

    ret

__cde80387FYL2X endp
end
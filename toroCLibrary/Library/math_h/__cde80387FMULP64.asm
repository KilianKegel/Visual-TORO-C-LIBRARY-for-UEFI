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
;    __cde80387FMULP64.asm
;
;Abstract:
;
;   CDE internal math 80387 instruction FMULP to calculate x * y.
;
;       double __cde80387FMULP(double x, double y)
;
;Author:
;
;    Kilian Kegel
;
;--*/
.code
__cde80387FMULP proc

    local float64X:QWORD
    local float64Y:QWORD

    movsd float64X,xmm0
    movsd float64Y,xmm1

    fld float64Y
    fld float64X

    FMULP

    fstp float64X

    movsd xmm0,float64X

    ret

__cde80387FMULP endp
end
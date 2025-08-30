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
;    __cde80387FMULP32.asm
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
    .model flat
.code
__cde80387FMULP proc C public float64X:QWORD, float64Y:QWORD

    fld float64Y
    fld float64X

    FMULP

    ret

__cde80387FMULP endp
end
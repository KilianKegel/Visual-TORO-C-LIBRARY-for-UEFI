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
;    __cde80387FSCALE32.asm
;
;Abstract:
;
;   CDE internal math 80387 instruction FSCALE to calculate Z = Y * 2^X.
;
;       double __cde80387SCALE(double x, double y)
;
;Author:
;
;    Kilian Kegel
;
;--*/
    .model flat
.code
__cde80387FSCALE proc C public float64X:QWORD, float64Y:QWORD

    fld float64Y
    fld float64X

    FSCALE
    fstp    st(1)

    ret

__cde80387FSCALE endp
end
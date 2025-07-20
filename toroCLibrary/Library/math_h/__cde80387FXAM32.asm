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
;    __cde80387FXAM32.asm
;
;Abstract:
;
;   CDE internal math 80387 instruction FXAM to analyse floating point number.
;   The function returns the status word.
;
;       unsigned short __cde80387FXAM(double x)
;
;Author:
;
;    Kilian Kegel
;
;--*/
    .model flat
.code
__cde80387FXAM proc C public float64:QWORD

    fld float64

    FXAM
    FSTSW ax
    FSTP st(0)

    ret

__cde80387FXAM endp
end
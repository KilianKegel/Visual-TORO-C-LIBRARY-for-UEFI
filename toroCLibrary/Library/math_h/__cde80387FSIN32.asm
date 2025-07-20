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
;    __cde80387FSIN32.asm
;
;Abstract:
;
;    CDE internal math 80387 instruction FSIN to calculate sine.
;
;       double __cde80387FSIN(double x)
;
;Author:
;
;    Kilian Kegel
;
;--*/
    .model flat
.code
__cde80387FSIN proc C public float64:QWORD

    fld float64

    FSIN

    ret

__cde80387FSIN endp
end
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
;    __cde80387FCOS64.asm
;
;Abstract:
;
;    CDE internal math 80387 instruction FCOS to calculate cosine.
;
;       double __cde80387FCOS(double x)
;
;Author:
;
;    Kilian Kegel
;
;--*/
.code
__cde80387FCOS proc

    local float64:QWORD

    movsd float64,xmm0

    fld float64

    FCOS

    fstp float64

    movsd xmm0,float64

    ret

__cde80387FCOS endp
end
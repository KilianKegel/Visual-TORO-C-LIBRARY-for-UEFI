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
;    __cde80387FINIT32.asm
;
;Abstract:
;
;   CDE internal math 80387 instruction FINIT to initilize FPU.
;
;       void __cde80387FINIT(void)
;
;Author:
;
;    Kilian Kegel
;
;--*/
    .model flat
.code
__cde80387FINIT proc C public

    FINIT

    ret

__cde80387FINIT endp
end
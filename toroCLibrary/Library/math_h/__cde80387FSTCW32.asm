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
;    __cde80387FSTCW32.asm
;
;Abstract:
;
;    CDE internal math 80387 instruction FSTSCW to store control word.
;
;       double __cde80387FSTSCW(double x)
;
;Author:
;
;    Kilian Kegel
;
;--*/
    .model flat
.code
__cde80387FSTCW proc C

    local cword:WORD

    FSTCW cword

    mov ax, cword

    ret

__cde80387FSTCW endp
end
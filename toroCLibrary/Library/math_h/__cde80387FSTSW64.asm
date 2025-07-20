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
;    __cde80387FSTSW64.asm
;
;Abstract:
;
;    CDE internal math 80387 instruction FSTSW to store status word.
;
;       double __cde80387FSTSW(double x)
;
;Author:
;
;    Kilian Kegel
;
;--*/

.code
__cde80387FSTSW proc

    FSTSW ax

    ret

__cde80387FSTSW endp
end
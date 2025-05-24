;/*++
;
;    toro C Library
;    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
;
;    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
;    SPDX-License-Identifier: GNU General Public License v3.0 only
;
;Module Name:
;
;    _CdeGetCurrentPrivilegeLevel32.asm
;
;Abstract:
;
;    CDE internal get code segment privilege level
;
;--*/

    .586p

    .model flat,C
    .code

__cdeGetEFLAGS proc near public 

    pushfd
    pop eax
    ret

__cdeGetEFLAGS endp
end
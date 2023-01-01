;/*++
;
;    toro C Library
;    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
;
;    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
;    SPDX-License-Identifier: GNU General Public License v3.0 only
;
;Module Name:
;
;    _CdeGetCurrentPrivilegeLevel64.asm
;
;Abstract:
;
;    CDE internal get code segment privilege level
;
;--*/
    .code
__cdeGetCurrentPrivilegeLevel proc public

    xor ax,ax           ; in UEFI 64 Bit always RING0
    ret

__cdeGetCurrentPrivilegeLevel endp
end
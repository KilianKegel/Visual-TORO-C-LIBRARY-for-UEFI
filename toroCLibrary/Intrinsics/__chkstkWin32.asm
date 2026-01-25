;/*++
;
;    toro C Library
;    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
;
;    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
;    SPDX-License-Identifier: GNU General Public License v3.0 only
;
;Module Name:
;
;   __chkstkWin32.asm
;
;Abstract:
;
;    Microsoft intrinsic function
;
;Author:
;
;   Kilian Kegel
;
;--*/
    .model flat
    .code
___chkstkWindows proc public

        push        ecx
        lea         ecx,[esp+4]
        sub         ecx,eax
        sbb         eax,eax
        not         eax
        and         ecx,eax
        mov         eax,esp
        and         eax,0FFFFF000h
cs10:
        cmp         ecx,eax
        jb          cs20
        mov         eax,ecx
        pop         ecx
        xchg        eax,esp
        mov         eax,dword ptr [eax]
        mov         dword ptr [esp],eax
        ret
cs20:
        sub         eax,1000h
        test        dword ptr [eax],eax
        jmp         cs10

___chkstkWindows endp
end
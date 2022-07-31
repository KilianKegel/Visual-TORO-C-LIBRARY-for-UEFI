;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   __chkstk()
;
;Module Name:
;
;   __chkstkWin64.asm
;
;Abstract:
;
;    Microsoft intrinsic function, disassembled from LIBCMT.lib
;
;Author:
;
;   Microsoft Corporation
;   One Microsoft Way
;   Redmond, WA 98052-6399
;   USA
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
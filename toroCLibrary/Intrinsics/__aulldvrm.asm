;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   64Bit signed integer division with remainder for 32Bit mode
;
;Module Name:
;
;   __aulldvrm.asm
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
    .386
    .model flat
.code
__aulldvrm proc near public
        push        esi
        mov         eax,dword ptr [esp+14h]
        or          eax,eax
        jne         L1
        mov         ecx,dword ptr [esp+10h]
        mov         eax,dword ptr [esp+0Ch]
        xor         edx,edx
        div         ecx
        mov         ebx,eax
        mov         eax,dword ptr [esp+8]
        div         ecx
        mov         esi,eax
        mov         eax,ebx
        mul         dword ptr [esp+10h]
        mov         ecx,eax
        mov         eax,esi
        mul         dword ptr [esp+10h]
        add         edx,ecx
        jmp         L2
L1:
        mov         ecx,eax
        mov         ebx,dword ptr [esp+10h]
        mov         edx,dword ptr [esp+0Ch]
        mov         eax,dword ptr [esp+8]
L3:
        shr         ecx,1
        rcr         ebx,1
        shr         edx,1
        rcr         eax,1
        or          ecx,ecx
        jne         L3
        div         ebx
        mov         esi,eax
        mul         dword ptr [esp+14h]
        mov         ecx,eax
        mov         eax,dword ptr [esp+10h]
        mul         esi
        add         edx,ecx
        jb          L4
        cmp         edx,dword ptr [esp+0Ch]
        ja          L4
        jb          L5
        cmp         eax,dword ptr [esp+8]
        jbe         L5
L4:
        dec         esi
        sub         eax,dword ptr [esp+10h]
        sbb         edx,dword ptr [esp+14h]
L5:
        xor         ebx,ebx
L2:
        sub         eax,dword ptr [esp+8]
        sbb         edx,dword ptr [esp+0Ch]
        neg         edx
        neg         eax
        sbb         edx,0
        mov         ecx,edx
        mov         edx,ebx
        mov         ebx,ecx
        mov         ecx,eax
        mov         eax,esi
        pop         esi
        ret         10h
__aulldvrm endp
end
;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   64Bit signed integer division with remainder for 32Bit mode
;
;Module Name:
;
;   __alldvrm.asm
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
__alldvrm proc near public
        push        edi
        push        esi
        push        ebp
        xor         edi,edi
        xor         ebp,ebp
        mov         eax,dword ptr [esp+14h]
        or          eax,eax
        jge         L1
        inc         edi
        inc         ebp
        mov         edx,dword ptr [esp+10h]
        neg         eax
        neg         edx
        sbb         eax,0
        mov         dword ptr [esp+14h],eax
        mov         dword ptr [esp+10h],edx
L1:
        mov         eax,dword ptr [esp+1Ch]
        or          eax,eax
        jge         L2
        inc         edi
        mov         edx,dword ptr [esp+18h]
        neg         eax
        neg         edx
        sbb         eax,0
        mov         dword ptr [esp+1Ch],eax
        mov         dword ptr [esp+18h],edx
L2:
        or          eax,eax
        jne         L3
        mov         ecx,dword ptr [esp+18h]
        mov         eax,dword ptr [esp+14h]
        xor         edx,edx
        div         ecx
        mov         ebx,eax
        mov         eax,dword ptr [esp+10h]
        div         ecx
        mov         esi,eax
        mov         eax,ebx
        mul         dword ptr [esp+18h]
        mov         ecx,eax
        mov         eax,esi
        mul         dword ptr [esp+18h]
        add         edx,ecx
        jmp         L4
L3:
        mov         ebx,eax
        mov         ecx,dword ptr [esp+18h]
        mov         edx,dword ptr [esp+14h]
        mov         eax,dword ptr [esp+10h]
L5:
        shr         ebx,1
        rcr         ecx,1
        shr         edx,1
        rcr         eax,1
        or          ebx,ebx
        jne         L5
        div         ecx
        mov         esi,eax
        mul         dword ptr [esp+1Ch]
        mov         ecx,eax
        mov         eax,dword ptr [esp+18h]
        mul         esi
        add         edx,ecx
        jb          L6
        cmp         edx,dword ptr [esp+14h]
        ja          L6
        jb          L7
        cmp         eax,dword ptr [esp+10h]
        jbe         L7
L6:
        dec         esi
        sub         eax,dword ptr [esp+18h]
        sbb         edx,dword ptr [esp+1Ch]
L7:
        xor         ebx,ebx
L4:
        sub         eax,dword ptr [esp+10h]
        sbb         edx,dword ptr [esp+14h]
        dec         ebp
        jns         L9
        neg         edx
        neg         eax
        sbb         edx,0
L9:
        mov         ecx,edx
        mov         edx,ebx
        mov         ebx,ecx
        mov         ecx,eax
        mov         eax,esi
        dec         edi
        jne         L8
        neg         edx
        neg         eax
        sbb         edx,0
L8:
        pop         ebp
        pop         esi
        pop         edi
        ret         10h
__alldvrm endp
end
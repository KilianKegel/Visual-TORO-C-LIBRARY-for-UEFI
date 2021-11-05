;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   64Bit signed integer multiplication for 32Bit mode
;
;Module Name:
;
;   __allmul.asm
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
__allmul proc near public
mov         eax,dword ptr [esp+8]
        mov         ecx,dword ptr [esp+10h]
        or          ecx,eax
        mov         ecx,dword ptr [esp+0Ch]
        jne         hard
        mov         eax,dword ptr [esp+4]
        mul         ecx
        ret         10h
hard:
        push        ebx
        mul         ecx
        mov         ebx,eax
        mov         eax,dword ptr [esp+8]
        mul         dword ptr [esp+14h]
        add         ebx,eax
        mov         eax,dword ptr [esp+8]
        mul         ecx
        add         edx,ebx
        pop         ebx
        ret         10h
__allmul endp
end
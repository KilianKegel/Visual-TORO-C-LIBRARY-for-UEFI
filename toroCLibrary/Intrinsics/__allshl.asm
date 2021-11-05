;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   64Bit left shift for 32Bit mode
;
;Module Name:
;
;   __allshl.asm
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
__allshl proc near public
        cmp         cl,40h
        jae         RETZERO
        cmp         cl,20h
        jae         MORE32
        shld        edx,eax,cl
        shl         eax,cl
        ret
MORE32:
        mov         edx,eax
        xor         eax,eax
        and         cl,1Fh
        shl         edx,cl
        ret
RETZERO:
        xor         eax,eax
        xor         edx,edx
        ret
__allshl endp
end
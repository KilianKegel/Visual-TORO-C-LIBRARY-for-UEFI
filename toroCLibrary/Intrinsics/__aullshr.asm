;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   64Bit unsigned right shift for 32Bit mode
;
;Module Name:
;
;   __aullshr.asm
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
__aullshr proc near public
        cmp         cl,40h
        jae         RETZERO
        cmp         cl,20h
        jae         MORE32
        shrd        eax,edx,cl
        shr         edx,cl
        ret
MORE32:
        mov         eax,edx
        xor         edx,edx
        and         cl,1Fh
        shr         eax,cl
        ret
RETZERO:
        xor         eax,eax
        xor         edx,edx
        ret
__aullshr endp
end
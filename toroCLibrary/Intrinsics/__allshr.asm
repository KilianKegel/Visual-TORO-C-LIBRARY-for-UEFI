;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   64Bit signed right shift for 32Bit mode
;
;Module Name:
;
;   __allshr.asm
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
__allshr proc near public
        cmp         cl,40h
        jae         RETSIGN
        cmp         cl,20h
        jae         MORE32
        shrd        eax,edx,cl
        sar         edx,cl
        ret
MORE32:
        mov         eax,edx
        sar         edx,1Fh
        and         cl,1Fh
        sar         eax,cl
        ret
RETSIGN:
        sar         edx,1Fh
        mov         eax,edx
        ret
__allshr endp
end
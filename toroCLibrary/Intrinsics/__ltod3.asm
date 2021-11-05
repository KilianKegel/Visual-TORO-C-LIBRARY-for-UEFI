;/*++
;
;   Copyright (c) Microsoft. All rights reserved.
;   
;   Convert long to double
;
;Module Name:
;
;   __ltod3.asm
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
    .XMM
.data
_Int32ToUInt32 label QWORD
    QWORD 0000000000000000h, 41f0000000000000h
.code
__ltod3 proc public

        xorps       xmm0,xmm0
        xorps       xmm1,xmm1
        cvtsi2sd    xmm0,ecx
        cvtsi2sd    xmm1,edx
        shr         ecx,1Fh
        mulsd       xmm1,mmword ptr [_Int32ToUInt32+8]
        addsd       xmm0,mmword ptr _Int32ToUInt32[ecx*8]
        
        addsd       xmm0,xmm1
        ret
__ltod3 endp
end
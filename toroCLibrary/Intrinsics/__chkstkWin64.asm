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
    .code
__chkstkWindows proc public

        sub         rsp,10h  
        mov         qword ptr [rsp],r10  
        mov         qword ptr [rsp+8],r11  
        xor         r11,r11  
        lea         r10,[rsp+18h]  
        sub         r10,rax  
        cmovb       r10,r11  
        mov         r11,qword ptr gs:[10h]  
        cmp         r10,r11  
        jae         cs20
        and         r10w,0F000h  
cs10:   lea         r11,[r11-1000h]  
        mov         byte ptr [r11],0  
        cmp         r10,r11  
        jne         cs10
cs20:   mov         r10,qword ptr [rsp]  
        mov         r11,qword ptr [rsp+8]  
        add         rsp,10h  
        ret

__chkstkWindows endp
end
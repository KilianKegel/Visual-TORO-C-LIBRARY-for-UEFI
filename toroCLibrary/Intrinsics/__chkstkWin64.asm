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
;   __chkstkWin64.asm
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
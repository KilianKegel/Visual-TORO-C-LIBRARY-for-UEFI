;/*++
;
;    toro C Library
;    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
;
;    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
;    SPDX-License-Identifier: GNU General Public License v3.0 only
;
;Module Name:
;
;    longjmp64.asm
;
;Abstract:
;
;    Implementation of the Standard C function longjump() for 64Bit
;
;--*/
    .code
    public longjmp
REGISTERS struct

    _xmm6   XMMWORD ?
    _xmm7   XMMWORD ?
    _xmm8   XMMWORD ?
    _xmm9   XMMWORD ?
    _xmm10  XMMWORD ?
    _xmm11  XMMWORD ?
    _xmm12  XMMWORD ?
    _xmm13  XMMWORD ?
    _xmm14  XMMWORD ?
    _xmm15  XMMWORD ?

    _rax    QWORD ? 	
    _rbx    QWORD ? 	
    _rbp    QWORD ? 	
    _rsi    QWORD ? 	
    _rdi    QWORD ? 	
    _r12    QWORD ? 	
    _r13    QWORD ? 	
    _r14    QWORD ? 	
    _r15    QWORD ?
    _rsp    QWORD ?
    _ret    QWORD ?

    _mxcsr  DWORD ? 	
    _fpucw  WORD  ?


REGISTERS ends

;0000000140001034  mov         edx,0AAh  
;0000000140001039  lea         rcx,[ixit_buf (0140025BE0h)]  

;000000014001A9AA  ldmxcsr     dword ptr [rcx+58h]  
;000000014001A9AE  fnclex  
;000000014001A9B0  fldcw       word ptr [rcx+5Ch]  

longjmp proc

;pBuf:QWORD,val:QWORD

    mov rax,[rcx + REGISTERS._rax]  
    mov rbx,[rcx + REGISTERS._rbx]  
    mov rbp,[rcx + REGISTERS._rbp]  
    mov rsi,[rcx + REGISTERS._rsi]  
    mov rdi,[rcx + REGISTERS._rdi]  
    mov r12,[rcx + REGISTERS._r12]  
    mov r13,[rcx + REGISTERS._r13]  
    mov r14,[rcx + REGISTERS._r14]  
    mov r15,[rcx + REGISTERS._r15]  

    mov rsp,[rcx + REGISTERS._rsp]

    ldmxcsr [rcx + REGISTERS._mxcsr]
    fnclex
    fldcw   [rcx + REGISTERS._fpucw]

    movdqu  xmm6 ,[rcx + REGISTERS._xmm6 ]
    movdqu  xmm7 ,[rcx + REGISTERS._xmm7 ]
    movdqu  xmm8 ,[rcx + REGISTERS._xmm8 ]
    movdqu  xmm9 ,[rcx + REGISTERS._xmm9 ]
    movdqu  xmm10,[rcx + REGISTERS._xmm10]
    movdqu  xmm11,[rcx + REGISTERS._xmm11]
    movdqu  xmm12,[rcx + REGISTERS._xmm12]
    movdqu  xmm13,[rcx + REGISTERS._xmm13]
    movdqu  xmm14,[rcx + REGISTERS._xmm14]
    movdqu  xmm15,[rcx + REGISTERS._xmm15]

    cmp rdx,0
    jne @f
    inc rdx
@@: mov rax,rdx
    mov rdx,[rcx + REGISTERS._ret]
    jmp rdx

longjmp endp
end

;   mov rcx,[pBuf]
;   mov rdx,[val]
;
;   mov ebx,[edx + REGISTERS._ebx]
;   mov ecx,[edx + REGISTERS._ecx]
;   mov edi,[edx + REGISTERS._edi]
;   mov esi,[edx + REGISTERS._esi]
;   mov ebp,[edx + REGISTERS._ebp]
;   mov esp,[edx + REGISTERS._esp]
;   .if eax == 0
;       mov eax,1
;   .endif
;   mov rdx,[rdx + REGISTERS._ret]
;   jmp rdx


;.if rdx == 0
;   inc rdx
;.endif
;
;.if [edx + REGISTERS._eax] == 0
;never run here
;
;000000014001A977  test        rdx,rdx  
;000000014001A97A  jne         LJ10 (014001A97Fh)  
;000000014001A97C  inc         rdx  
;000000014001A97F  xor         r10,r10  
;000000014001A982  cmp         qword ptr [rcx],r10  
;000000014001A985  jne         LJ10+94h (014001AA13h)  
;000000014001A98B  mov         rax,rdx  

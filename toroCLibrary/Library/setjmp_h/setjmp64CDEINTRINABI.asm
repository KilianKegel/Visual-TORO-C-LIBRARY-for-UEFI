;/*++
;
;    toro C Library
;    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
;
;    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
;    SPDX-License-Identifier: GNU General Public License v3.0 only
;
;Module Name:
;
;    setjmp64.asm
;
;Abstract:
;
;    Implementation of the Standard C function setjump() for 64Bit
;
;--*/
    .code
    public _setjmp
    public __imp_setjmp
    public __cdeSETJMPCDEINTRINABIAnchor
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

_setjmp proc
; pBuf:QWORD

;   mov     rcx,[pBuf]

    mov     [rcx + REGISTERS._rax],rax
    mov     [rcx + REGISTERS._rbx],rbx
    mov     [rcx + REGISTERS._rbp],rbp
    mov     [rcx + REGISTERS._rsi],rsi
    mov     [rcx + REGISTERS._rdi],rdi
    mov     [rcx + REGISTERS._r12],r12
    mov     [rcx + REGISTERS._r13],r13
    mov     [rcx + REGISTERS._r14],r14
    mov     [rcx + REGISTERS._r15],r15

    lea     r8,[rsp+8]  
    mov     [rcx + REGISTERS._rsp],r8

    mov     r8,qword ptr [rsp]  
    mov     [rcx + REGISTERS._ret],r8
    
    stmxcsr [rcx + REGISTERS._mxcsr]
    fnstcw  [rcx + REGISTERS._fpucw]

    movdqu  [rcx + REGISTERS._xmm6 ],xmm6
    movdqu  [rcx + REGISTERS._xmm7 ],xmm7
    movdqu  [rcx + REGISTERS._xmm8 ],xmm8
    movdqu  [rcx + REGISTERS._xmm9 ],xmm9
    movdqu  [rcx + REGISTERS._xmm10],xmm10
    movdqu  [rcx + REGISTERS._xmm11],xmm11
    movdqu  [rcx + REGISTERS._xmm12],xmm12
    movdqu  [rcx + REGISTERS._xmm13],xmm13
    movdqu  [rcx + REGISTERS._xmm14],xmm14
    movdqu  [rcx + REGISTERS._xmm15],xmm15

    sub eax,eax

    ret
_setjmp endp

__imp_setjmp dq _setjmp

__cdeSETJMPCDEINTRINABIAnchor LABEL NEAR

end
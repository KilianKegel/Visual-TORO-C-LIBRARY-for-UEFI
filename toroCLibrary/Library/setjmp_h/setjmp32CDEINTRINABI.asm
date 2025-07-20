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
;    setjmp32.asm
;
;Abstract:
;
;    Implementation of the Standard C function setjump() for 32Bit
;
;--*/

    .586p

    .model flat,C
    .code
    public _setjmp
    public _setjmp3
    public _imp__setjmp
    public _imp__setjmp3
    public __cdeSETJMPCDEINTRINABIAnchor
REGISTERS struct
    _ebx    DWORD ?
    _ecx    DWORD ?
    _edi    DWORD ?
    _esi    DWORD ?
    _ebp    DWORD ?
    _esp    DWORD ?
    _ret    DWORD ?
REGISTERS ends
_setjmp3 label near
_setjmp proc near pBuf:DWORD

    mov edx,[pBuf]

    mov [edx + REGISTERS._ebx], ebx
    mov [edx + REGISTERS._ecx], ecx
    mov [edx + REGISTERS._edi], edi
    mov [edx + REGISTERS._esi], esi

    mov eax,dword ptr [esp] ;ebp

    mov [edx + REGISTERS._ebp], eax
    mov [edx + REGISTERS._esp], esp

    add [edx + REGISTERS._esp], 8

    mov eax,dword ptr [esp + 4];ret address
    mov [edx + REGISTERS._ret], eax

    sub eax,eax
    ret
_setjmp endp

_imp__setjmp   dd _setjmp
_imp__setjmp3  dd _setjmp

__cdeSETJMPCDEINTRINABIAnchor LABEL NEAR

end
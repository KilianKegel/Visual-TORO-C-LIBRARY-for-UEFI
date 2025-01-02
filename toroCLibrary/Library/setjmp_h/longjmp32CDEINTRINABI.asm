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
;    longjmp32.asm
;
;Abstract:
;
;    Implementation of the Standard C function longjump() for 32Bit
;
;--*/
    .586p

    .model flat, C
    .code
    public longjmp
    public _imp__longjmp
    public __cdeLONGJMPCDEINTRINABIAnchor
REGISTERS struct
    _ebx    DWORD ?
    _ecx    DWORD ?
    _edi    DWORD ?
    _esi    DWORD ?
    _ebp    DWORD ?
    _esp    DWORD ?
    _ret    DWORD ?
REGISTERS ends

longjmp proc near pBuf:DWORD,val:DWORD

    mov edx,[pBuf]
    mov eax,[val]

    mov ebx,[edx + REGISTERS._ebx]
    mov ecx,[edx + REGISTERS._ecx]
    mov edi,[edx + REGISTERS._edi]
    mov esi,[edx + REGISTERS._esi]
    mov ebp,[edx + REGISTERS._ebp]
    mov esp,[edx + REGISTERS._esp]
    .if eax == 0
        mov eax,1
    .endif
    mov edx,[edx + REGISTERS._ret]
    jmp edx
   ret
longjmp endp

_imp__longjmp dd longjmp

__cdeLONGJMPCDEINTRINABIAnchor LABEL NEAR

end
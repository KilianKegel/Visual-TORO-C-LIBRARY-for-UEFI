;###################################################################################
;### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
;###################################################################################
;/*!
;\copyright
;
;    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.
;
;\mainpage __restore_rt
;
;\brief Implementation of the <em>__restore_rt</em> function
;
;\file __restore_rt.asm
;
;*/
    .code

__restore_rt proc

  db 048h,0c7h,0c0h,00fh,000h,000h,000h	; mov    rax,0xf
  db 00fh,005               	        ; syscall 
  db 00fh,01fh,080h,000h,000h,000h,000 	; nop    DWORD PTR [rax+0x0]

__restore_rt endp

end
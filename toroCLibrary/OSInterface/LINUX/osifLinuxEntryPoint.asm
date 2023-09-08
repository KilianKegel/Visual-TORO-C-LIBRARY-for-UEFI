;###################################################################################
;### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
;###################################################################################
;/*!
;\copyright
;
;    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.
;
;\mainpage osifLinuxEntryPointOS
;
;\brief Implementation of the <em>_MainEntryPointLinux</em> function
;
;\file osifLinuxEntryPointOS.asm
;
;*/
    .code

extrn _osifLinuxEntryPointIntern:near

_cdeCRT0Linux proc public

    mov rcx,rsp
    add rsp,-32
    call _osifLinuxEntryPointIntern

_cdeCRT0Linux endp
end
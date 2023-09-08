;###################################################################################
;### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
;###################################################################################
;/*!
;\copyright
;
;    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.
;
;\mainpage _syscall
;
;\brief Implementation of the <em>_syscall</em> function
;
;   https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux#syscall
;   https://docs.microsoft.com/en-us/cpp/build/x64-software-conventions?redirectedfrom=MSDN&view=vs-2019
;Syscall # 	Param 1 	Param 2 	Param 3 	Param 4 	Param 5 	Param 6
;    rax 	rdi 	    rsi 	    rdx 	    r10 	    r8 	        r9 
;           nonvol      nonvol      vol         vol         vol         vol
;    rcx    rdx         r8          r9          parm5       parm6       parm7
;
;\file _syscall.asm
;
;*/
    .code

_syscall proc

    push        rbp  
    mov         rbp,rsp

    add         rsp,0FFFFFFFFFFFFFFF0h  
    mov         qword ptr [rbp-8],rsi  
    mov         qword ptr [rbp-10h],rdi  
    mov         rax,rcx  
    mov         rdi,rdx  
    mov         rsi,r8  
    mov         rdx,r9  
    mov         r10,qword ptr [rbp+30h]  
    mov         r8,qword ptr [rbp+38h]  
    mov         r9,qword ptr [rbp+40h]  

    syscall
    
    mov         rsi,qword ptr [rbp-8]  
    mov         rdi,qword ptr [rbp-10h]  
    
    leave
    ret

_syscall endp

end
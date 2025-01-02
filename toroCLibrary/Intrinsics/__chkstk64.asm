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
;    __chkstk64.asm
;
;Abstract:
;
;   Microsoft specific helper function
;   https://docs.microsoft.com/en-us/windows/win32/devnotes/-win32-chkstk
;   Adjusted for CDE usage.
;
;--*/
    public __chkstk
    public __cdeChkStkAddr

    .data

    ;
    ; NOTE: __chkstkWindows is filled here by the Windows64 _MainEntryPointWinNT
    ;
__cdeChkStkAddr qword 0         
    
    .code

__chkstk proc

    cmp [__cdeChkStkAddr], 0
    je  exit
    jmp [__cdeChkStkAddr]
exit:
    ret

__chkstk endp

end
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
;    __chkstk32.asm
;
;Abstract:
;
;   Microsoft specific helper function
;   https://docs.microsoft.com/en-us/windows/win32/devnotes/-win32-chkstk
;   Adjusted for CDE usage.
;
;--*/

    .386
    .model flat
.code
__chkstk proc near public

        ret

__chkstk endp
end
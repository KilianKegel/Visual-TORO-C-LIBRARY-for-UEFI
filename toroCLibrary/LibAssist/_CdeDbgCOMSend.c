/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeDbgCOMSend.c

Abstract:

    CDE internal COM port send

Author:

    Kilian Kegel

--*/
int _CdeDbgCOMSend(short c) {

    #define IODELAY _cdeOUTByte(0xED, 0x1),_cdeOUTByte(0xED, 0x2),_cdeOUTByte(0xED, 0x3),_cdeOUTByte(0xED, 0x4),_cdeOUTByte(0xED, 0x5),_cdeOUTByte(0xED, 0x6),_cdeOUTByte(0xED, 0x7),_cdeOUTByte(0xED, 0x8)
    
    _cdeOUTByte(0x3F8, (char)c);

    IODELAY;

    return 0;
}
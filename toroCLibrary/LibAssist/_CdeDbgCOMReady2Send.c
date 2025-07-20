/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeDbgCOMReady2Send.c

Abstract:

    CDE internal check COM port to be ready to send

Author:

    Kilian Kegel

--*/
int _CdeDbgCOMReady2Send(void) {
    #define IODELAY _cdeOUTByte(0xED, 0x1),_cdeOUTByte(0xED, 0x2),_cdeOUTByte(0xED, 0x3),_cdeOUTByte(0xED, 0x4),_cdeOUTByte(0xED, 0x5),_cdeOUTByte(0xED, 0x6),_cdeOUTByte(0xED, 0x7),_cdeOUTByte(0xED, 0x8)

    int nRet;

    nRet = 0x60 == (0x60 & _cdeINByte(0x3FD));
    
    IODELAY;

    return nRet;
}

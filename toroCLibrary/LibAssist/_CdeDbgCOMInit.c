/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeDbgCOMInit.c

Abstract:

    CDE internal COM port initialization

Author:

    Kilian Kegel

--*/
int _CdeDbgCOMInit(void) {

    int nRet = 0;
#define IODELAY _cdeOUTByte(0xED, 0x1),_cdeOUTByte(0xED, 0x2),_cdeOUTByte(0xED, 0x3),_cdeOUTByte(0xED, 0x4),_cdeOUTByte(0xED, 0x5),_cdeOUTByte(0xED, 0x6),_cdeOUTByte(0xED, 0x7),_cdeOUTByte(0xED, 0x8)

    do {
        IODELAY;
        if (0xFF == _cdeINByte(0x3FD)) //check LSR / line status register
            break;

        IODELAY;
        if (0x55 != _cdeINByte(0x3FF)) {

            IODELAY;

            _cdeOUTByte(0x3FB, 0x80); IODELAY;
            _cdeOUTByte(0x3F8, 0x01); IODELAY;
            _cdeOUTByte(0x3F9, 0x00); IODELAY;
            _cdeOUTByte(0x3FB, 0x03); IODELAY;

            _cdeOUTByte(0x3FF, 0x55); IODELAY;
        }

        nRet = 1;

    } while (0);

    return nRet;
}

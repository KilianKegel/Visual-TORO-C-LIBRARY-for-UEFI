/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeMemPutChar.c

Abstract:

    CDE internal PutChar() for 8Bit memory with common interface:

        void (*pfnDevPutChar)(int c, void** ppDest)

Author:

    Kilian Kegel

--*/
void _CdeMemPutChar(int c, void** ppDest) 
{
    unsigned char* pDest = *ppDest;

    *pDest++ = (unsigned char)c;
    *ppDest = pDest;
}

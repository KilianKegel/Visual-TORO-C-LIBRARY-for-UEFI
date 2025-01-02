/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeMemPutWChar.c

Abstract:

    CDE internal PutChar() for 16Bit memory with common interface:

        void (*pfnDevPutChar)(int c, void** ppDest)

Author:

    Kilian Kegel

--*/
void _CdeMemPutWChar(int c, void** ppDest) 
{
    unsigned short* pDest = *ppDest;

    *pDest++ = (unsigned short)c;
    *ppDest = pDest;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeMemGetChar.c

Abstract:

    CDE internal GetChar() for 8Bit memory with common interface:
    
        int (*pfnDevGetChar)(void** ppSrc)

Author:

    Kilian Kegel

--*/
int _cdeMemGetChar(void** ppSrc) 
{
    unsigned char* pSrc = *ppSrc;
    int nRet;
    
    nRet = (unsigned char)*pSrc++;
    *ppSrc = pSrc;

    return nRet;
}

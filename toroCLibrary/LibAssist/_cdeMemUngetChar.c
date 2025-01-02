/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeMemUngetChar.c

Abstract:

    CDE internal Ungetc() for memory

Author:

    Kilian Kegel

--*/
int _cdeMemUngetChar(int c, void** ppSrc) 
{
    unsigned char* pDst = *ppSrc;
    //
    //NOTE: with "/GF enable read-only string pooling"  memory location is R/O. Don't really put back the char
    //

    --pDst;
    *ppSrc = pDst;

    return c;
}

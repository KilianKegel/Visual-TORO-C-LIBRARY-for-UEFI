/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeMemPutNada.c

Abstract:

    CDE internal PutChar() for nothing with common interface:

        void (*pfnDevPutChar)(int c, void** ppDest)


Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
void _CdeMemPutNada(int c, void** ppDest) 
{
    return;
}

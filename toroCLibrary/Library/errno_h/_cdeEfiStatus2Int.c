/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeEfiStatus2int.c

Abstract:

    CDE internal EFI_STATUS value sizing to integer

Author:

    Kilian Kegel

--*/
#include <stddef.h>

int _cdeEfiStatus2Int(size_t Status)
{
    int nRet = (int)Status;
    long long errbitINT64 = (1LL << (8 * sizeof(Status) - 1));

    if (errbitINT64 & Status)
        nRet |= (1 << (8 * sizeof(int) - 1));

    return nRet;
}

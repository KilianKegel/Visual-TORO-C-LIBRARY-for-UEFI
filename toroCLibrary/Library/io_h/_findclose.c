/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _findclose.c

Abstract:

    Microsoft C Library specific function _findclose()

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "__cdeFindFirst.h"

/** _findclose()
* 
Synopsis

    #include <io.h>
    int _findclose(intptr_t handle);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findclose?view=msvc-160

Paramters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findclose?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findclose?view=msvc-160#return-value

**/
int _findclose(intptr_t hFile)
{
    CDEFINDFIRSTNEXT* pcdeFindFirstNextData = (CDEFINDFIRSTNEXT*)hFile;
    int nRet = 0;

    free(pcdeFindFirstNextData->pCdeFileInfo);
    
    return nRet;
}

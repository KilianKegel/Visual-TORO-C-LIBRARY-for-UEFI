/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeFindFirst.h

Abstract:

    Toro C Library specific datastructure for Microsoft C _findfirst()

Author:

    Kilian Kegel

--*/
#ifndef __CDE_FINDFIRST_H_
#define __CDE_FINDFIRST_H_

#include <CdeServices.h>

typedef struct tagCDEFINDFIRSTNEXT
{
    CDEFILEINFO* pCdeFileInfo;  // pointer to be free on findclose()
    char* pstrSearchPatNAME;    // pointer to be free on findclose()
    char* pstrSearchPatEXT;     // pointer to be free on findclose()
    int nCountOfAll;
}CDEFINDFIRSTNEXT;

#endif//__CDE_FINDFIRST_H_

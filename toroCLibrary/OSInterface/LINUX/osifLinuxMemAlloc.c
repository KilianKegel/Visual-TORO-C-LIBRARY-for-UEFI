//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxMemAlloc

\brief Implementation of the <em>osifLinuxMemAlloc</em> function

\file osifLinuxMemAlloc.c

*/
#include <stdio.h>
#include <CdeServices.h>

//
//  prototype
//
OSIFMEMALLOC _osifLinuxMemAlloc; 

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

HEAPDESC* _osifLinuxMemAlloc(CDE_APP_IF* pCdeAppIf, size_t Pages) {

    HEAPDESC* pMemory = NULL, *pTmp = NULL;

    pMemory = (void*)_syscall(12, 0, 0, 0, 0, 0, 0);

    pTmp = (void*)_syscall(12, (intptr_t)pMemory + 4096 * Pages, 0, 0, 0, 0, 0);

    if (NULL == pTmp)
        pMemory = NULL;

    if (pMemory)
        pMemory->fInalterable = TRUE;

    return pMemory;
}
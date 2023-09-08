//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileWrite

\brief Implementation of the <em>osifLinuxFileWrite</em> function

\file osifLinuxFileWrite.c

*/
#include <stdio.h>
#include <CdeServices.h>

//
// prototype
//
OSIFFWRITE _osifLinuxFileWrite;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

size_t _osifLinuxFileWrite(CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile)
{
    int64_t num = 0LL;

    num = _syscall(/* sys_write */ 1, (int64_t)pCdeFile->emufp, (int64_t)ptr, (int64_t)nelem, 0, 0, 0);

    return num < 0LL ? 0LL : (size_t)num;
}

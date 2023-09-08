//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileRead

\brief Implementation of the <em>osifLinuxFileRead</em> function

\file osifLinuxFileRead.c

*/
#include <stdio.h>
#include <CdeServices.h>
//
// prototype
//
OSIFFREAD _osifLinuxFileRead;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

size_t _osifLinuxFileRead(CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile)
{
    int64_t num = 0LL;

    //printf(__FILE__"(%d)" __FUNCTION__ "(): size %d, fp %p, fd %llX\n", __LINE__, (int)nelem, pCdeFile, pCdeFile->emufp);

    num = _syscall(/* sys_read */ 0, (int64_t)pCdeFile->emufp, (int64_t)ptr, nelem, 0, 0, 0);

    //printf(__FILE__"(%d)" __FUNCTION__ "(): num %lld\n", __LINE__, num);

    return num < 0LL ? 0LL : (size_t)num;
}

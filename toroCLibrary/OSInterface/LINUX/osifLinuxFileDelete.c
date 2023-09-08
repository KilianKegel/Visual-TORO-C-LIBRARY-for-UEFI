//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileDelete

\brief Implementation of the <em>osifLinuxFileDelete</em> function

\file osifLinuxFileDelete.c

*/
#include <CdeServices.h>
//
// protptype
//
OSIFFDELETE _osifLinuxFileDelete;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

int _osifLinuxFileDelete(CDE_APP_IF* pCdeAppIf, const char* filename, CDEFILE* pCdeFile)
{
    int64_t nRet;

    nRet = _syscall(/* sys_unlink */ 87, (int64_t)filename, 0, 0, 0, 0, 0);

    return 0 == nRet ? 0 : -1;
}
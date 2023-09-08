//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileClose

\brief Implementation of the <em>osifLinuxFileClose</em> function

\file osifLinuxFileClose.c

*/
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
//
// protptype
//
OSIFFCLOSE _osifLinuxFileClose;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

int _osifLinuxFileClose(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile)
{
    int64_t nRet;

    nRet = _syscall(/* sys_close */ 3, (int64_t)pCdeFile->emufp,0 ,0 , 0, 0, 0);

    return 0LL == nRet ? 0 : EOF;
}

//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxGetTscPerSec

\brief Implementation of the <em>osifLinuxGetTscPerSec</em> function

\file osifLinuxGetTscPerSec.c

*/
#include <time.h>
#include <CdeServices.h>
//
// prototype
//
OSIFGETTSCPERSEC _osifLinuxGetTscPerSec;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

/** GetTscPerSec

    GetTscPerSec() returns the Performance counter counts per second
    @param[in] void

    @retval number of CPU clock per second

**/

uint64_t _osifLinuxGetTscPerSec(CDE_APP_IF* pCdeAppIf) {

    return 1000000;
}

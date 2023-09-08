//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxGetTsc

\brief Implementation of the <em>osifLinuxGetTsc</em> function

\file osifLinuxGetTsc.c

*/
#include <time.h>
#include <CdeServices.h>
//
// prototype
//
OSIFGETTSC  _osifLinuxGetTsc;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

/** GetTsc

    GetTsc() returns the Performance timer counts per second
    @param[in] void

    @retval number of CPU clock per second

**/
uint64_t _osifLinuxGetTsc(CDE_APP_IF* pCdeAppIf) {

    struct __kernel_timespec {
        time_t          tv_sec;                 /* seconds */
        long long       tv_nsec;                /* nanoseconds */
    }TSpec = { 0LL, 0LL };
    
    int64_t rtusec, rtsec; //runtime µSec, runtime Sec
    int64_t nRet;
    

    nRet = _syscall(/* sys_clock_gettime */ 228, /*CLOCK_REALTIME_COARSE*/5LL, (int64_t)&TSpec, 0, 0, 0, 0);

    rtsec = TSpec.tv_sec - pCdeAppIf->pCdeServices->TimeAtSystemStart;
    rtusec = 1000000 * rtsec;
    rtusec = rtusec + TSpec.tv_nsec / 1000;

    return 0 == nRet ? (uint64_t)rtusec : 0ULL;

}

//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxGetTime

\brief Implementation of the <em>osifLinuxGetTime</em> function

\file osifLinuxGetTime.c

*/
#include <time.h>
#include <CdeServices.h>

/** GetRtcTime

    GetRtcTime() returns the current time of the system in seconds
    since epoch.

    @param[in] void

    @retval seconds since 1.1.1970

**/
OSIFGETTIME	_osifLinuxGetTime;   // prototype

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

int64_t _osifLinuxGetTime(IN CDE_APP_IF* pCdeAppIf) {

    struct tm tm, *ptm = &tm;
    int64_t nRet = 0;

    nRet = _syscall(/* sys_time */ 201, 0, 0, 0, 0, 0, 0);

    //printf("%s(%d): nRet %llX\n", __FILE__, __LINE__, nRet);

    ptm = localtime(&nRet);
    //printf("%s(%d): %04d/%02d/%02d, %02d:%02d:%02d\n",__FILE__,__LINE__,1900 + ptm2->tm_year,1 + ptm2->tm_mon,ptm2->tm_mday,ptm2->tm_hour,ptm2->tm_min,ptm2->tm_sec);

    return nRet;
}

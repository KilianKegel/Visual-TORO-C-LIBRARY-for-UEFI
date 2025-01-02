/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    timeCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Gets the system time.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
typedef long long time_t;

/** time() -- string format time

Synopsis
    #include <time.h>
    time_t time(time_t *timer);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160
    The time function determines the current calendar time.
    The encoding of the value is unspeciﬁed.
Returns
    The time function returns the implementation’s best approximation to the current calendar time.
    The value (time_t)(-1) is returned if the calendar time is not available.
    If timer is not a null pointer, the return value is also assigned to the object it points to.

    @param[in] time_t *ptimer

    @retval time_t, calendar time

**/
static time_t _time64CDEABI(time_t* ptimer)
{

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    time_t TSC = pCdeAppIf->pCdeServices->pGetTsc(pCdeAppIf);
    time_t STA = pCdeAppIf->pCdeServices->TSClocksAtSystemStart;
    time_t TSCF = pCdeAppIf->pCdeServices->TSClocksPerSec;
    time_t SecSinceStart = (TSC - STA) / TSCF;

    time_t t = pCdeAppIf->pCdeServices->TimeAtSystemStart + SecSinceStart;

    if (NULL != ptimer)
        *ptimer = t;
    return t;
}

static time_t timeCDEABI(time_t* ptimer) {

    return _time64CDEABI(ptimer);

}

MKCDEABI(_time64);
MKCDEABI(time);
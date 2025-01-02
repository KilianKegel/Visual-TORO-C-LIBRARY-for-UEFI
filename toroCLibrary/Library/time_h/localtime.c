/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    localtime.c

Abstract:

    Implementation of the Standard C function.
    Converts a time value and corrects for the local time zone.

Author:

    Kilian Kegel

--*/
#define localtime msft_localtime/*rename original localtime() to establish my own localtime()*/
#include <CdeServices.h>
#include <time.h>
#undef localtime

extern void* __cdeGetAppIf(void);
extern struct tm* _cdeGmltime(const long long* timep, struct tm* ptm);

/**
Synopsis
    #include <time.h>
    struct tm* localtime(const long long* timep) ;
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160#return-value
**/
struct tm* _localtime64(time_t const* timep)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    struct tm* pTm = (struct tm*) & pCdeAppIf->StructTmLocalTimeStaticInternal[0];

    return _cdeGmltime(timep, pTm);
}

struct tm* localtime(const long long* timep) {
    return _localtime64(timep);
}

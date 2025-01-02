/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    gmtime.c

Abstract:

    Implementation of the Standard C function.
    Converts a time_t time value to a tm structure.

Author:

    Kilian Kegel

--*/
#define gmtime msft_gmtime/*rename original gmtime() to establish my own gmtime()*/
#include <CdeServices.h>
#include <time.h>
#undef gmtime

extern void* __cdeGetAppIf(void);
extern struct tm* _cdeGmltime(const long long* timep, struct tm* ptm);

/**
Synopsis
    #include <time.h>
    struct tm* gmtime(const long long* timep);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160#return-value
**/
struct tm* gmtime(const long long* timep)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    struct tm* pTm = (struct tm*) & pCdeAppIf->StructTmGmTimeStaticInternal[0];

    return _cdeGmltime(timep, pTm);
}

struct tm* _gmtime64(const long long* timep) {
    return gmtime(timep);
}

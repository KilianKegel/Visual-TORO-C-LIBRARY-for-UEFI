/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeSig2idx.c

Abstract:

    Toro C library internal helper function convert signal number to branch dispatcher index

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <signal.h>

#define ELC(x) (sizeof(x) / sizeof(x[0]))

/** 

Synopsis
    char _cdeSig2idx(int sig)
Description
    function converts the signal number to an index into the rgfnSignal[]
    branch dispatcher table with that order:
        SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT, SIGABRT_COMPAT
Returns
    @param[in] sig signal number

    @retval index into internal branch dispatcher
    @retval -1 if invalid

**/
char _cdeSig2idx(int sig) {

    static char sigorder[CDE_SIGNAL_NUM] = { SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT, SIGABRT_COMPAT };
    unsigned i;

    for (i = 0; i < ELC(sigorder); i++)
        if (sigorder[i] == (char)sig)
            break;

    return i == ELC(sigorder) ? -1 : (char)i;
}

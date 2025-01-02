/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeSigIgn.c

Abstract:

    Toro C library internal default signal ignore handler

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <signal.h>

/**

Synopsis

    void _cdeSigIgn(int sig)

Description

    this is the ignore signal handler for

        SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT, SIGABRT_COMPAT

Returns

    @param[in] sig signal number
    @retval void

**/
void _cdeSigIgn(int sig) {

    return;
}

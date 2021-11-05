/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    SIGNAL.h

Abstract:

    SIGNAL.H for toro C Library

Author:

    Kilian Kegel

--*/
#ifndef _CDE_SIGNAL_H_
#define _CDE_SIGNAL_H_

//#pragma warning( disable : 4996 4311 4312 4101)

typedef int sig_atomic_t;

#define SIG_DFL		(void(*)(int))0
#define SIG_IGN		(void(*)(int))1
#define SIG_ERR		(void(*)(int))-1

#define SIGABRT			22
#define SIGFPE			8
#define SIGILL			4
#define SIGINT			2
#define SIGSEGV			11
#define SIGTERM			15
#define SIGBREAK		21  // MSFT specific Ctrl-Break sequence
#define SIGABRT_COMPAT  6   // MSFT specific SIGABRT compatible with other platforms, same as SIGABRT

void ( *signal( int sig, void ( *func )( int ) ) )( int );
int raise( int sig );

#endif//_CDE_SIGNAL_H_

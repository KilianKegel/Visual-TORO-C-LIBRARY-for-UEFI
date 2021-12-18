/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    signal.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=258
Author:

    Kilian Kegel

--*/
#ifndef _CDE_SIGNAL_H_
#define _CDE_SIGNAL_H_

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

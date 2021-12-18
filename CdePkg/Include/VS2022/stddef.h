/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    stddef.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=254

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STDDEF_H_
#define _CDE_STDDEF_H_

#ifndef NULL
	#define NULL ((void *)0)
#endif//NULL

#pragma warning( disable : 4142 )
#if   defined(_M_AMD64)
	typedef   signed long long ptrdiff_t;
	typedef unsigned long long size_t;
#else
	typedef  signed  int ptrdiff_t;
	typedef unsigned int size_t;
#endif

typedef short wchar_t;

#define offsetof(s,m) ((size_t)&(((s*)0)->m))

#endif//_CDE_STDDEF_H_

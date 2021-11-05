/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    STDDEF.h

Abstract:

    STDDEF.H for toro C Library

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STDDEF_H_
#define _CDE_STDDEF_H_

//#pragma warning( disable : 4996 4311 4312 4101)

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

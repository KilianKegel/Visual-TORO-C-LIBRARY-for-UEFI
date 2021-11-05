/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    ASSERT.h

Abstract:

    ASSERT.H for toro C Library

Author:

    Kilian Kegel

--*/
#ifndef _CDE_ASSERT_H_
#define _CDE_ASSERT_H_
#include <stddef.h>

#undef assert

#ifdef NDEBUG

	#define assert(exp) ((void)0)

#else
	#define _CDE_CRT_WIDE_(s) L ## s
	#define _CDE_CRT_WIDE(s) _CDE_CRT_WIDE_(s)

	void __cdecl _wassert(wchar_t const* _Message, wchar_t const* _File,unsigned _Line);
	#define assert(exp) (void)((!!(exp)) || (_wassert(_CDE_CRT_WIDE(#exp), _CDE_CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0))

	//removed _assert() used in older CRT versions, support _wassert() only
	//#define assert(exp) (void)( (!!(exp)) || (_assert(#exp, __FILE__, __LINE__), 0) )

#endif

#endif//_CDE_ASSERT_H_

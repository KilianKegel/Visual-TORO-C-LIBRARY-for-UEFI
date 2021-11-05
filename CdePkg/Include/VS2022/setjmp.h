/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    SETJMP.h

Abstract:

    SETJMP.H for toro C Library

Author:

    Kilian Kegel

--*/
#ifndef _CDE_SETJMP_H_
#define _CDE_SETJMP_H_

//#pragma warning( disable : 4996 4311 4312 4101)

#ifdef jmp_buf
#undef jmp_buf
#endif//ndef jmp_buf
#if   defined(_M_AMD64)
	#define JMPBUF_DWORD_SIZE (256/4)	// internal register symbols not provided on C level, as microsoft does
#else
	#define JMPBUF_DWORD_SIZE (64/4)	// internal register symbols not provided on C level, as microsoft does
#endif

typedef unsigned jmp_buf[JMPBUF_DWORD_SIZE];

int _setjmp( jmp_buf env );
void longjmp( jmp_buf env, int val );

#define setjmp(env) _setjmp(env)

#endif//_CDE_SETJMP_H_


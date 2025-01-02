/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _except1.c

Abstract:

    Microsoft specific intrinsic function supplement

Author:

    Kilian Kegel

--*/
#define FILE void
extern FILE* __acrt_iob_func(unsigned i);
#define stderr (__acrt_iob_func(2))

extern void _cdeAbort(void);

extern __declspec(dllimport) int fprintf(FILE* stream, const char* pszFormat, ...);

void _except1(void)
{

    fprintf(stderr, "math error\n");

    _cdeAbort();

}

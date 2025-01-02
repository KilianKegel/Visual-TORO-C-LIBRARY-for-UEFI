/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    VwprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Writes formatted output by using a pointer to a list of arguments.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void
#define stdout (__acrt_iob_func(1))

extern FILE* __acrt_iob_func(unsigned i);
extern __declspec(dllimport) int vfwprintf(FILE* stream, const wchar_t* pszFormat, va_list ap);

/**
Synopsis
    #include <wchar.h>
    int vwprintf(const wchar_t* pszFormat, va_list ap);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vprintf-vprintf-l-vwprintf-vwprintf-l?view=msvc-160
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=377
    The vwprintf function is equivalent to wprintf, with the variable argument list
    replaced by arg, which shall have been initialized by the va_start macro (and
    possibly subsequent va_arg calls). The vwprintf function does not invoke the
    va_end macro.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vprintf-vprintf-l-vwprintf-vwprintf-l?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vprintf-vprintf-l-vwprintf-vwprintf-l?view=msvc-160#return-value
**/
static int vwprintfCDEABI(const wchar_t* pszFormat, va_list ap) {

    return vfwprintf(stdout, pszFormat, ap);
}

MKCDEABI(vwprintf);
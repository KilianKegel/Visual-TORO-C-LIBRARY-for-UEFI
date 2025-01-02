/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Swscanf.c

Abstract:

    Implementation of the Standard C function.
    Read formatted data from a wide string.

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern __declspec(dllimport) int vfwscanf(FILE* stream, const wchar_t* pwcsFormat, va_list ap);

/**

Synopsis
#include <wchar.h>
    int wscanf(const wchar_t *format, ...);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=378
    Reads formatted data from the standard input stream.
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l#return-value
**/
static int wscanfCDEABI(const wchar_t* pwcsFormat, ...) {

    va_list ap;
    int nRet;

    va_start(ap, pwcsFormat);

    nRet = vfwscanf((void*)CDE_STDIN, pwcsFormat, ap);

    va_end(ap);

    return nRet;

}


MKCDEABI(wscanf);
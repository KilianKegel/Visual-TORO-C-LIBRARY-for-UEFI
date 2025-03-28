/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FputsCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Writes a string to a stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
#define FILE void

extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);

/** fputs
Synopsis
    #include <stdio.h>
    int fputs(const char * restrict s,FILE * restrict stream);
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=309
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fputs-fputws?view=msvc-160&viewFallbackFrom=vs-2019
    The fputs function writes the string pointed to by s to the stream pointed to by
    stream. The terminating null character is not written.
Returns
    The fputs function returns EOF if a write error occurs; otherwise it returns a
    nonnegative value

NOTE:   Seems, that Microsoft has not implemented any errorreporting.
        Even when writing to a write protected file a 0 == success is returned.
    @param[in] const char * s

    @param[in] FILE *stream

    @retval 0 on success

    @retval EOF on error.

**/
static int fputsCDEABI(const char* s, FILE* stream) {

    return EOF != fwrite(s, 1, strlen(s), stream) ? 0 : EOF;
}

MKCDEABI(fputs);
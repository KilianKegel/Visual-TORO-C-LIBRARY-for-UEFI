/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FputwcCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Writes a character to a stream.

Author:

    Kilian Kegel

--*/
#include <assert.h>
#undef NULL
#include <CdeServices.h>
//
// stdio.h
//
#define WEOF ((wint_t)(0xFFFF))
#define FILE void

extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);

/** fputc
Synopsis
    #include <wchar.h>
    wint_t fputwc(wchar_t c,FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fputc-fputwc?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=380
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fputc-fputwc?view=msvc-160#return-value
**/
static wint_t fputwcCDEABI(wchar_t c, FILE* stream) {
    CDEFILE* pCdeFile = (CDEFILE*)stream;
    wint_t nRet = WEOF;
    size_t size = 1ULL + (0 != (pCdeFile->openmode & O_BINARY)); // 2 bin, 1 txt

    do {

        if(1 == size)
            if (c > 0xFF)
                break;

        if (1 != fwrite(&c, size, 1, stream))
            break;
        nRet = c;

    } while (0);

    return nRet;
}

MKCDEABI(fputwc);
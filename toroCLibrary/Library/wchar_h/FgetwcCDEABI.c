/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FgetwcCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Read a wide character from a stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
#define WEOF ((wint_t)(0xFFFF))
#define FILE void

extern void (*pinvalid_parameter_handlerCDEABI)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
extern __declspec(dllimport) size_t fread(const void* ptr, size_t size, size_t nelem, FILE* stream);

/** fgetwc
Synopsis
    #include <wchar.h>
    wint_t fgetwc(FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetc-fgetwc?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=379
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetc-fgetwc?view=msvc-160#return-value
**/
static wint_t fgetwcCDEABI(FILE* stream) {

    wint_t nRet = WEOF;

    if (NULL == stream) {
        //(*pinvalid_parameter_handlerCDEABI)(L"\"NULL == stream\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
        (*pinvalid_parameter_handlerCDEABI)(NULL, NULL, NULL, 0, 0);
    }
    else
        do {
            CDEFILE* pCdeFile = (CDEFILE*)stream;
            wint_t c = 0;
            size_t size = 1ULL + (0 != (pCdeFile->openmode & O_BINARY)); // 2 bin, 1 txt
            wint_t msk = 1 == size ? 0xFF : 0xFFFF;

            if (1 != fread(&c, size, 1, stream))
                break;
            nRet = msk & c;

        } while (0);

    return nRet;
}

MKCDEABI(fgetwc);
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FgetwsCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Get a wide string from a stream.

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

/** fgets
Synopsis
#include <wchar.h>
    wchar_t *fgetws(wchar_t *str, int numChars, FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=379
    The fgetws function is identical to fgets except that it gets a string of multibyte characters (if
    present) from the input stream pointed to by fp, converts them to wide characters, and stores them in the
    wide-character array pointed to by buf. In this case, n specifies the number of wide characters, less one,
    to be read.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-160#return-value
**/
static wchar_t* fgetwsCDEABI(wchar_t* s, int n, FILE* stream) {

    int i = 0;

    if (NULL == stream
        || NULL == s
        || 0 > n) {
        //(*pinvalid_parameter_handlerCDEABI)(L"\"NULL == stream || NULL == s || 0 > n\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
        (*pinvalid_parameter_handlerCDEABI)(NULL, NULL, NULL, 0, 0);
    }
    else
        do {

            CDEFILE* pCdeFile = (CDEFILE*)stream;
            wchar_t c;
            size_t size = 1ULL + (0 != (pCdeFile->openmode & O_BINARY)); // 2 bin, 1 txt
            wint_t msk = 1 == size ? 0xFF : 0xFFFF;

            for (i = 0, c = 0; i < (n - 1) && c != '\n'; i++) {

                if (1 != fread(&c, size, 1, stream))
                    break;

                s[i] = msk & c;
            }

            if (i != 0)
                s[i] = '\0';

        } while (0);

    return i == 0 ? NULL : s;
}

MKCDEABI(fgetws);
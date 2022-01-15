/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fgetwc.c

Abstract:

    Implementation of the Standard C function.
    Read a wide character from a stream.

Author:

    Kilian Kegel

--*/
#include <cdeservices.h>
#include <stdio.h>

extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);

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
wint_t fgetwc(FILE* stream) {

    wint_t nRet = WEOF;

    if (NULL == stream)
        (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);
    else
        do {
            CDEFILE* pCdeFile = (CDEFILE*)stream;
            wint_t c;
            size_t size = 1 + (0 != (pCdeFile->openmode & O_BINARY)); // 2 bin, 1 txt
            wint_t msk = 1 == size ? 0xFF : 0xFFFF;

            if (1 != fread(&c, size, 1, stream))
                break;
            nRet = msk & c;

        } while (0);

    return nRet;
}
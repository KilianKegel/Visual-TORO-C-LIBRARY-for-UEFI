/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FerrorCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Tests for an error on a stream.

TODO: 
    invoke invalid parameter handler on NULL FILE pointer

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern int __cdeIsFilePointer(void* stream);

/** clearerr

Synopsis
    #include <stdio.h>
    int ferror(FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ferror?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ferror?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ferror?view=msvc-160#return-value

**/
static int ferrorCDEABI(FILE* stream) {

    CDEFILE* pCdeFile = (CDEFILE*)stream;
    //
    //TODO: invoke invalid parameter handler on NULL FILE pointer
    //
    return __cdeIsFilePointer(pCdeFile) ? (int)pCdeFile->fErr : 0;
}

MKCDEABI(ferror);
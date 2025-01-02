/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FwriteCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Writes data to a stream.
    CTRL-Z, and text vs. binary mode is supported to be Microsoft C Library compatible

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#define EOF    (-1)
#define FILE void
#define BUFSIZ  512

extern int __cdeIsFilePointer(void* stream);

__declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
__declspec(dllimport) void* malloc(size_t size);

/**
Synopsis

    #include <stdio.h
    size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite?view=msvc-160&viewFallbackFrom=vs-2019
    The fwrite function writes, from the array pointed to by ptr, up to nmemb elements
    whose size is specified by size, to the stream pointed to by stream. For each object,
    size calls are made to the fputc function, taking the values (in order) from an array of
    unsigned char exactly overlaying the object. The file position indicator for the
    stream (if defined) is advanced by the number of characters successfully written. If an
    error occurs, the resulting value of the file position indicator for the stream is
    indeterminate.
Paramters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite?view=msvc-160#return-value
    The fwrite function returns the number of elements successfully written, which will be
    less than nmemb only if a write error is encountered. If size or nmemb is zero,
    fwrite returns zero and the state of the stream remains unchanged

**/
static size_t fwriteCDEABI(const void* ptr, size_t size, size_t nelem, FILE* stream) {

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return pCdeAppIf->pCdeServices->pFWriteCORE(pCdeAppIf, ptr, size, nelem, stream);
}

MKCDEABI(fwrite);

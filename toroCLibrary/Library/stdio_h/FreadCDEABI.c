/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FreadCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Reads data from a stream.
    CTRL-Z, and text vs. binary mode is supported to be Microsoft C Library compatible

Author:

    Kilian Kegel

--*/
#include <limits.h>
#include <stddef.h>
#include <CdeServices.h>

//
// stdio.h
//
#define EOF    (-1)
#define FILE void
#define BUFSIZ  512

extern int __cdeIsFilePointer(void* stream);
extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void rewind(FILE* stream);
extern __declspec(dllimport) int fflush(FILE* stream);

/**
Synopsis

    #include <stdio.h
    size_t fread(void *ptr, size_t size, size_t nelem, FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=313
    The function reads characters from the input stream stream and stores
    them in successive elements of the array whose first element has the
    address (char *)ptr until the function stores size*nelem characters or
    sets the end-of-file or error indicator. It returns n/size, where n is the
    number of characters it read. If n is not a multiple of size, the value
    stored in the last element is indeterminate. If the function sets the error
    indicator, the file-position indicator is indeterminate.
Paramters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-160#return-value
**/
static size_t freadCDEABI(const void* ptr, size_t size, size_t nelem, FILE* stream) {

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return pCdeAppIf->pCdeServices->pFReadCORE(pCdeAppIf, ptr, size, nelem, stream);
}

MKCDEABI(fread);
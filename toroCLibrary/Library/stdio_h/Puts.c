/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Puts.c

Abstract:

    Implementation of the Standard C function.
    Writes a string to stdout.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <string.h>
#include <CdeServices.h>

/** puts
Synopsis
    #include <stdio.h>
    int puts(const char *s);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/puts-putws?view=msvc-160&viewFallbackFrom=vs-2019
    The puts function writes the string pointed to by s to the stream pointed to by stdout,
    and appends a new-line character to the output. The terminating null character is not
    written.
Returns
    The puts function returns EOF if a write error occurs; otherwise it returns a nonnegative
    value.

    **/
int puts(const char* s) {
    size_t nRet = 0;
    char c = '\n';
    do {
        nRet = fwrite(s, 1, strlen(s), (FILE*)CDE_STDOUT);
        if (nRet == (size_t)EOF)
            break;
        nRet = fwrite(&c, 1, 1, (FILE*)CDE_STDOUT);
    } while (0);

    fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);    //flush to stdout

    return nRet != (size_t)EOF ? 0 : EOF;
}
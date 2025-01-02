/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    GetsCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Gets a line from the stdin stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
#define FILE void

__declspec(dllimport) int fgetc(FILE* stream);

/** gets
Synopsis
    #include <stdio.h>
    char *gets(char *s);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/gets-getws?view=msvc-160&viewFallbackFrom=vs-2019
    The gets function reads characters from the input stream pointed to by
    stdin, into the array pointed to by s, until  end-of-file  is  encountered
    or a new-line character is read. Any new-line character is discarded, and
    a null character is written immediately after the last character read into the array.
Returns
    The gets function returns s if successful. If end-of-file is encountered and no
    characters have been read into the array, the contents of the array remain unchanged and a
    null pointer is returned.  If a read error occurs during the operation, the array contents are
    indeterminate and a null pointer is returned.
**/
static char* getsCDEABI(char* s) {

    int n = -1;
    int c;

    do {

        c = fgetc((FILE*)CDE_STDIN);

        if (c != EOF && c != '\n') {
            s[++n] = (char)c;
        }

    } while (c != EOF && c != '\n');

    if (n != -1 || c == '\n')
        s[++n] = '\0';

    return n != -1 ? s : NULL;
}

MKCDEABI(gets);
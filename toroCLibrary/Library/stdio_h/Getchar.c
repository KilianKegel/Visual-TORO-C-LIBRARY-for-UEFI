/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Getchar.c

Abstract:

    Implementation of the Standard C function.
    Reads a character from standard input.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>

#ifdef getchar
#   undef getchar
#endif//getchar

/** getchar
Synopsis
    #include <stdio.h>
    int getchar(void);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getchar-getwchar?view=msvc-160
    The getchar function is equivalent to getc with the argument stdin.
Returns
    The getchar function returns the next character from the input stream pointed to by
    stdin. If the stream is at end-of-file, the end-of-file indicator for the stream is set and
    getchar returns EOF. If a read error occurs, the error indicator for the stream is set and
    getchar returns EOF.
**/
int getchar(void)
{
    return fgetc((FILE*)CDE_STDIN);
}
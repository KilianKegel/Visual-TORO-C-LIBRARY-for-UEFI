/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Putchar.c

Abstract:

    Implementation of the Standard C function.
    Writes a character to stdout.

Author:

    Kilian Kegel

--*/
#include <stdio.h>

/**
Synopsis
    #include <stdio.h>
    int putchar(int c)
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/putchar-putwchar?view=msvc-160&viewFallbackFrom=vs-2019
    The putchar function is equivalent to putc with the second argument stdout.
    The putc function is equivalent to fputc in the Microsoft implementation.
    The fputc function writes the character specified by c (converted to an unsigned
    char) to the output stream pointed to by stream, at the position indicated by the
    associated file position indicator for the stream (if defined), and advances the indicator
    appropriately. If the file cannot support positioning requests, or if the stream was opened
    with append mode, the character is appended to the output stream.
Returns
    The fputc function returns the character written. If a write error occurs, the error
    indicator for the stream is set and fputc returns EOF.

**/
int putchar(int b)
{
    return fputc(b, stdout);
}
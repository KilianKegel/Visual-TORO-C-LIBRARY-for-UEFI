/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fputc.c

Abstract:

    Implementation of the Standard C function.
    Writes a character to a stream.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>

/** fputc
Synopsis
    #include <stdio.h>
    int fputc(int c, FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fputc-fputwc?view=msvc-160&viewFallbackFrom=vs-2019
    The fputc function writes the character specified by c (converted to an unsigned
    char) to the output stream pointed to by stream, at the position indicated by the
    associated file position indicator for the stream (if defined), and advances the indicator
    appropriately. If the file cannot support positioning requests, or if the stream was opened
    with append mode, the character is appended to the output stream.
Returns
    The fputc function returns the character written. If a write error occurs, the error
    indicator for the stream is set and fputc returns EOF.

    @param[in] int c

    @param[in] FILE *stream

    @retval character from the input stream.

    @retval EOF on error.

**/
int fputc(int b, FILE* stream) {
    unsigned nRet = (unsigned)EOF;
    unsigned char c = (unsigned char)b;

    do {

        if (1 != fwrite(&c, 1, 1, stream))
            break;
        nRet = (unsigned char)c;

    } while (0);

    return nRet;
}
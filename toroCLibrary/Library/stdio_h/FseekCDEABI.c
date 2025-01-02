/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FseekCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Set file position indicator.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern int _fseeki64(FILE* stream, __int64 offset, int mode);

/** Synopsis
    #include <stdio.h>
    int fseek(FILE *stream, long int offset, int whence);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/fseek-lseek-constants?view=msvc-160&viewFallbackFrom=vs-2019
    The fseek function sets the file position indicator for the stream pointed to by stream.
    If a read or write error occurs, the error indicator for the stream is set and fseek fails.
    For a binary stream, the new position, measured in characters from the beginning of the
    file, is obtained by adding offset to the position specified by whence. The specified
    position is the beginning of the file if whence is SEEK_SET, the current value of the file
    position indicator if SEEK_CUR, or end-of-file if SEEK_END. A binary stream need not
    meaningfully support fseek calls with a whence value of SEEK_END.
    For a text stream, either offset shall be zero, or offset shall be a value returned by
    an earlier successful call to the ftell function on a stream associated with the same file
    and whence shall be SEEK_SET.
    After determining the new position, a successful call to the fseek function undoes any
    effects of the ungetc function on the stream, clears the end-of-file indicator for the
    stream, and then establishes the new position. After a successful fseek call, the next
    operation on an update stream may be either input or output.
Returns
    The fseek function returns nonzero only for a request that cannot be satisfied.

**/
static int fseekCDEABI(FILE* stream, long offset, int mode) {

    return _fseeki64(stream, (long long)offset, mode);
}

MKCDEABI(fseek);
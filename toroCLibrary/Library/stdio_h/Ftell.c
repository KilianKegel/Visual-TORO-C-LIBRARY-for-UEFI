/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Ftell.c

Abstract:

    Implementation of the Standard C function.
    Get file position indicator.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>

extern __int64 _ftelli64(FILE* pCdeFile);

/*
Synopsis
    #include <stdio.h>
    long int ftell(FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ftell-ftelli64?view=msvc-160&viewFallbackFrom=vs-2019
    The ftell function obtains the current value of the file position indicator for the stream
    pointed to by stream. For a binary stream, the value is the number of characters from
    the beginning of the file. For a text stream, its file position indicator contains unspecified
    information, usable by the fseek function for returning the file position indicator for the
    stream to its position at the time of the ftell call; the difference between two such
    return values is not necessarily a meaningful measure of the number of characters written
    or read.
Returns
    If successful, the ftell function returns the current value of the file position indicator
    for the stream. On failure, the ftell function returns −1L and stores an
    implementation-defined positive value in errno.
*/
long ftell(FILE* pCdeFile)
{
    return (long)_ftelli64(pCdeFile);
}


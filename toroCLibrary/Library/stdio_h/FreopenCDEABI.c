/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FreopenCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Reassigns a file pointer.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern __declspec(dllimport) int fflush(FILE* stream);
extern __declspec(dllimport) FILE* fopen(const char* filename, const char* mode);
extern __declspec(dllimport) int fclose(FILE* stream);
extern __declspec(dllimport) void free(void* ptr);

/** freopen

Synopsis
    #include <stdio.h>
    FILE *freopen(const char * restrict filename,const char * restrict mode,FILE * restrict stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/freopen-wfreopen?view=msvc-160&viewFallbackFrom=vs-2019
    The freopen function opens the file whose name is the string pointed to by filename
    and associates the stream pointed to by stream with it. The mode argument is used just as
    in the fopen function.
    If filename is a null pointer, the freopen function attempts to change the mode of
    the stream to that specified by mode, as if the name of the file currently associated
    with the stream had been used. It is implementation-defined which changes of mode are
    permitted (if any), and under what circumstances.

    The freopen function first attempts to close any file that is associated with the specified
    stream. Failure to close the file is ignored. The error and end-of-file indicators for the
    stream are cleared.
Returns
    The freopen function returns a null pointer if the open operation fails. Otherwise,
    freopen returns the value of stream.

**/
static FILE* freopenCDEABI(const char* filename, const char* mode, FILE* stream) {

    CDEFILE* fp, * sp = (CDEFILE*)stream;

    fp = (CDEFILE*)fopen(filename, mode);

    if (NULL != fp) 
    {
        if(O_CDEREDIR & sp->openmode)   // don't close redirected I/O, just flush
            fflush(stream);
        else
            fclose(stream);

        *sp = *fp;

        fp->fRsv = FALSE;

        sp->openmode |= O_CDEREOPEN + O_CDEREDIR;
    }
    else
        stream = NULL;

    return stream;
}

MKCDEABI(freopen);
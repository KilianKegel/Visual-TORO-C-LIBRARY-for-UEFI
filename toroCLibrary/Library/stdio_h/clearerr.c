/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Clearerr.c

Abstract:

    Implementation of the Standard C function.
    Resets the error indicator for a stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdio.h>

extern int __cdeIsFilePointer(void* stream);

/** clearerr

Synopsis
    #include <stdio.h>
    void clearerr(FILE *stream);
Description
    The clearerr() function clears the end-of-file and error indicators for the stream pointed
    to by stream
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/clearerr?redirectedfrom=MSDN&view=msvc-160

Returns

    The clearerr() function returns no value.

// NOTE MSFT : clearerr(NULL / ) generates fastfail in Windows
//             It


    @param[in] FILE *stream

    @retval void

**/
void clearerr(FILE* stream) {
    CDEFILE* pCdeFile = (CDEFILE*)stream;

    if (__cdeIsFilePointer(pCdeFile)) {
        pCdeFile->fErr = 0;
        pCdeFile->fEof = 0;
    }

}

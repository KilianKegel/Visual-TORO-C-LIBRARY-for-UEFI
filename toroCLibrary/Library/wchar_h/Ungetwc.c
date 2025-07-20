/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Ungetc.c

Abstract:

    Implementation of the Standard C function.
    Pushes a character back onto the stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdio.h>
#include <wchar.h>

extern int __cdeIsFilePointer(void* stream);

/** ungetwc
Synopsis
    #include <stdio.h>
    #include <wchar.h>
    wint_t ungetwc(wint_t c, FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ungetc-ungetwc?view=msvc-160&viewFallbackFrom=vs-2019
    The ungetc function pushes the character specified by c (converted to an unsigned
    char) back onto the input stream pointed to by stream. Pushed-back characters will be
    returned by subsequent reads on that stream in the reverse order of their pushing. A
    successful intervening call (with the stream pointed to by stream) to a file positioning
    function (fseek, fsetpos, or rewind) discards any pushed-back characters for the
    stream. The external storage corresponding to the stream is unchanged.
    One character of pushback is guaranteed. If the ungetc function is called too many
    times on the same stream without an intervening read or file positioning operation on that
    stream, the operation may fail.

    If the value of c equals that of the macro EOF, the operation fails and the input stream is
    unchanged.

    A successful call to the ungetc function clears the end-of-file indicator for the stream.
    The value of the file position indicator for the stream after reading or discarding all
    pushed-back characters shall be the same as it was before the characters were pushed
    back. For a text stream, the value of its file position indicator after a successful call to the
    ungetc function is unspecified until all pushed-back characters are read or discarded.
    For a binary stream, its file position indicator is decremented by each successful call to
    the ungetc function; if its value was zero before a call, it is indeterminate after the
    call.250)
Returns
    The ungetc function returns the character pushed back after conversion, or EOF if the
    operation fails.

**/

wint_t ungetwc(wint_t c, FILE * stream) 
{
    CDEFILE* pCdeFile = (CDEFILE*)stream;
    unsigned int nRet = (unsigned int)EOF;

    if (__cdeIsFilePointer(pCdeFile))                               // check file pointer valid
    {
        if (O_TEXT == (pCdeFile->openmode & O_TEXT))                // in text mode deal with single byte chars only
        {
            if (0x100 > c)
                nRet = ungetc((int)c, stream);
        }
        else
            do {
                
                if (WEOF == c)                                      // check c valid
                    break;

                if (2 > pCdeFile->bidx )                            // check buffer can receive one wide character
                    break;
                
                pCdeFile->bidx -= (sizeof(wint_t));                 // decrement index

                *((wint_t*)(&pCdeFile->Buffer[pCdeFile->bidx])) = c;// store char in the buffer

                nRet = c;                                           // mark nRet for success with c

                pCdeFile->fEof = FALSE;                             // clear EOF on success

            } while (0);

    }
    return nRet == (unsigned)EOF ? WEOF : (wint_t)nRet;
}
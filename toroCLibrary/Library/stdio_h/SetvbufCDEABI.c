/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    SetvbufCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Controls stream buffering.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
#define FILE void
#define BUFSIZ  512
#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004

extern int __cdeIsFilePointer(void* stream);
extern __declspec(dllimport) void* malloc(size_t size);

/**

Synopsis
    #include <stdio.h>
    int setvbuf(FILE * stream,char * buf,int mode, size_t size);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setvbuf?view=msvc-160&viewFallbackFrom=vs-2019
    The setvbuf function may be used only after the stream pointed to by stream has
    been associated with an open file and before any other operation (other than an
    unsuccessful call to setvbuf) is performed on the stream. The argument mode
    determines how stream will be buffered, as follows: _IOFBF causes input/output to be
    fully buffered; _IOLBF causes input/output to be line buffered; _IONBF causes
    input/output to be unbuffered. If buf is not a null pointer, the array it points to may be
    used instead of a buffer allocated by the setvbuf function233) and the argument size
    specifies the size of the array; otherwise, size may determine the size of a buffer
    allocated by the setvbuf function. The contents of the array at any time are
    indeterminate.
Returns
    The setvbuf function returns zero on success, or nonzero if an invalid value is given
    for mode or if the request cannot be honored.

    NOTE MSFT:  1. wrong stream returns success(0) but nothing is done by setvbuf()
                2. wrong mode or size e.g. _IOLBF+_IONBF and -1 leads to return EOF
                3. stream has lower priority as mod and size
    NOTE CDE:   that behavior is implemented differently to MSFT:
                1. check stream, if wrong, return 0
                    then
                2. check the other parms, return 0/EOF as required

**/
static int setvbufCDEABI(FILE* stream, char* buf, int mode, size_t size)
{
    CDEFILE* pCdeFile = (CDEFILE*)stream;
    int nRet = 0;   //todo check on real windows

    //
    // check iobuf
    //
    do {
        if (mode & ~(_IONBF | _IOLBF | _IOFBF) ||/* other bit set */
           (mode & (_IONBF) && mode & (_IOLBF | _IOFBF))/* nobuffering simultanously with buffering*/)
        {
            nRet = EOF;
            break;
        }

        if (__cdeIsFilePointer(pCdeFile))
        {

            size = mode & _IONBF ? 1 : size;
            size = size == 0 ? 1 : size;

            if (NULL != pCdeFile->Buffer)    // if buffer aleady there, don't do anything
                break;

            if (NULL == buf)
            {

                pCdeFile->Buffer = malloc(size  /* [1] extend buffer range for possible access out of buffer range below*/);

                if (NULL == pCdeFile->Buffer) {
                    nRet = EOF;
                    break;
                }
            }
            else
                pCdeFile->Buffer = buf;

            if (NULL == pCdeFile->Buffer) {    // if buffer allocation has failed
                nRet = EOF;
                break;
            }

            pCdeFile->bsiz = (int)size;

        }
    } while (0);
    return nRet;
}

MKCDEABI(setvbuf);
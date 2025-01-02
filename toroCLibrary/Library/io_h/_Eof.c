/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Eof.c

Abstract:

    Microsoft C Library specific function _eof()
        NOTE: This is _not_ a POSIX C function!
    Tests for end of file (EOF).

Author:

    Kilian Kegel

NOTE: _eof() returns 1 only if file position is exactly EOF and not beyond

--*/
#include <io.h>
#include <stdio.h>
#include <errno.h>
#include <CdeServices.h>
extern int __cdeIsIOBuffer(void* stream);
extern int __cdeIsFilePointer(void* stream);
extern void _cdeAbort(void);

/** _eof()

Synopsis
    #include <io.h>
    int _eof(int fd);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/eof
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/eof.html
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/eof#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/eof#return-value

**/
int _eof(int fd)
{
    CDEFILE* pCdeFile = __cdeGetIOBuffer((unsigned)fd);
    FILE* fp = (void*)pCdeFile;
    long nRet = -1, posorg, size;
    int n;

    if (__cdeIsFilePointer(pCdeFile)) 
    {
        do {
            posorg = ftell(fp);

            n = fseek(fp, 0, SEEK_END);
            if (0 != n)
                break;                              //error

            size = ftell(fp);

            n = fseek(fp, posorg, SEEK_SET);
            if (0 != n)
                break;                              //error

            nRet = posorg == size;

        } while (0);
    }
    else {
        errno = EBADF;
    }

    return (int)nRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Lseek.c

Abstract:

    Implementation of the POSIX C function.
    Moves a file pointer to the specified location.

Author:

    Kilian Kegel

--*/
#include <io.h>
#include <stdio.h>
#include <errno.h>
#include <CdeServices.h>
extern int __cdeIsIOBuffer(void* stream);
extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
extern void _cdeAbort(void);

/** _lseek()

Synopsis
    #include <io.h>
    long _lseek(int fd, long offset, int origin);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/lseek-lseeki64
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/lseek.html
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/lseek-lseeki64#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/lseek-lseeki64#return-value

**/
long _lseek(int fd, long offset, int origin)
{
    FILE* fp = __cdeGetIOBuffer((unsigned)fd);
    long nRet = NULL == fp ? -1 : 0;
    long pos, posorg;
    int seekstat;

    if(nRet == 0) do 
    {
        posorg = ftell(fp);

        seekstat = fseek((FILE*)fp, offset, origin);
        pos = ftell(fp);
        
        nRet = pos;

        if (0 != seekstat)
            fseek((FILE*)fp, posorg, SEEK_SET);  // restore original position on error

        if (EINVAL == errno)
            nRet = -1;

    } while (0);

    return nRet;
}
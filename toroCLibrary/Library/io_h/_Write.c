/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Write.c

Abstract:

    Implementation of the POSIX C function.
    Writes data to a file.

Author:

    Kilian Kegel

--*/

#include <stdio.h>
#include <limits.h>
#include <CdeServices.h>

extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
extern void* __cdeGetIOBuffer(unsigned i);

/** close
Synopsis

    #include <io.h>
    int _write(int fd, const void *buffer, unsigned int count);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/write?view=msvc-170
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html

Parameters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/write?view=msvc-170#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/write?view=msvc-170#return-value

**/
int _write(int fd, const void* buffer, unsigned int count)
{
    CDEFILE* pCdeFile = __cdeGetIOBuffer((unsigned)fd);
    int nRet = -1;

    if (NULL != pCdeFile) do {

//TODO check    if (NULL == stream)
//TODO check        (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);

        if (NULL == buffer || count > INT_MAX)
        {
            //(*pinvalid_parameter_handler)(L"\"NULL == buffer || count > INT_MAX\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
            (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);
            break;
        }
        
        if (O_RDONLY == (pCdeFile->openmode & (O_WRONLY | O_RDWR))) {
            //TODO check ErrNo
            break;
        }

        nRet = (int)fwrite(buffer, 1, count, (FILE*)pCdeFile);

    } while (0);

    return nRet;
}

//int write(int fd, const void* buffer, unsigned int count)
//{
//        return write(fd, buffer, count);
//}

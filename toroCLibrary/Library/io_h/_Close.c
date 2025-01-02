/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Close.c

Abstract:

    Implementation of the POSIX C function.
    Closes a file.

Author:

    Kilian Kegel

--*/

#include <stdio.h>

extern void* __cdeGetIOBuffer(unsigned i);

/** close
Synopsis

    #include <io.h>
    int _close(int fd);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/close?view=msvc-170
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html

Parameters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/close?view=msvc-170#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/close?view=msvc-170#return-value

**/
int _close(int fd) 
{
    FILE* fp = __cdeGetIOBuffer((unsigned)fd);
    int nRet = NULL == fp ? -1 : 0;

    if(NULL != fp)
        fclose(fp);


    return nRet;
}

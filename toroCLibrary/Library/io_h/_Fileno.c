/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Fileno.c

Abstract:

    Implementation of the POSIX C function.
    Gets the file descriptor associated with a stream.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <errno.h>
#include <CdeServices.h>
extern int __cdeIsIOBuffer(void* stream);
extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
extern void _cdeAbort(void);

/** fileno

Synopsis
    #include <stdio.h>
    int fileno(FILE* stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fileno?view=msvc-170
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/fileno.html
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fileno?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fileno?view=msvc-170#return-value

**/
int _fileno(FILE* stream) 
{
    CDEFILE* pCdeFile = (CDEFILE*)stream;
    int nRet = -1,fdplusone;

    do 
    {

        if (NULL == pCdeFile)
        {
            //(*pinvalid_parameter_handler)(L"\"NULL == pCdeFile\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
            (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);
            errno = EINVAL;
            break;
        }

        fdplusone = __cdeIsIOBuffer(pCdeFile);

        if (0 == fdplusone)
            _cdeAbort();

        if (0 == pCdeFile->fRsv)
            break;

        nRet = -1 + fdplusone;      // get real fd index, 0-based

    } while (0);

    return nRet;
}

//int fileno(FILE* stream) 
//{
//    return fileno(stream);
//}

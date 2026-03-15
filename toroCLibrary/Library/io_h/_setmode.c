/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _setmode.c

Abstract:

    Microsoft C Library specific function _setmode()
    Sets the file translation mode.

    NOTE:   HACK->
            THIS FUNCTION IS NOT FULLY IMPLEMENTED, YET.
            IT CURRENTLY ONLY SUPPORTS THE _O_U16TEXT/_O_WTEXT MODE FLAG ON STDOUT/STDERR

Author:

    Kilian Kegel

--*/
#include <errno.h>
#include <CdeServices.h>


/** _setmode()
*
Synopsis

    #include <io.h>
    int _setmode (int fd, int mode);

Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode
Paramters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode#return-value

**/
int _setmode(int fd, int mode)
{
    int nRet = -1;                  // default error for _setmode()
    CDEFILE* pCdeFile = __cdeGetIOBuffer((unsigned)fd);

    do 
    {
        if (!((fd == 1) | (fd == 2) | (NULL != pCdeFile )))
        {
            errno = EBADF; /* The file descriptor specified by fd is invalid. */
            break;
        }

        nRet = pCdeFile->openmode & 0xFFFFF;        // mask O_CDEWCSZONLY (1 << (16 + 4)) and higher bits, 
                                                    // because they are not part of the openmode, but used for internal flags in CDEFILE

        pCdeFile->openmode &= ~(_O_U16TEXT | _O_WTEXT);
        pCdeFile->openmode |= mode;

    } while (0);

    return nRet;
}

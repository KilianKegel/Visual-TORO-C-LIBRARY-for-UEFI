/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Fdopen.c

Abstract:

    Implementation of the POSIX C function.
    Associates a stream with a file that was previously opened for low-level I/O.

Author:

    Kilian Kegel

TODO: check compatibility

--*/
//#undef NCDETRACE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <CdeServices.h>

extern void* __cdeGetIOBuffer(unsigned i);

static struct _tblMode {
    const char* pszMode;
    int openmode;
    //        BYTE bmDOSAccType;  //  000 read only
    //                            //  001 write only
    //                            //  010 read/write
}tblMode[] = {
    {"r"        ,O_RDONLY |                         O_TEXT   },
    {"rb"       ,O_RDONLY |                         O_BINARY },
    {"r+"       ,O_RDWR   |                         O_TEXT   },
    {"r+b"      ,O_RDWR   |                         O_BINARY },
    {"rb+"      ,O_RDWR   |                         O_BINARY },
    {"w"        ,O_WRONLY | O_TRUNC  |  O_CREAT |   O_TEXT   },
    {"wb"       ,O_WRONLY | O_TRUNC  |  O_CREAT |   O_BINARY },
    {"w+"       ,O_RDWR   | O_TRUNC  |  O_CREAT |   O_TEXT   },
    {"w+b"      ,O_RDWR   | O_TRUNC  |  O_CREAT |   O_BINARY },
    {"wb+"      ,O_RDWR   | O_TRUNC  |  O_CREAT |   O_BINARY },
    {"a"        ,O_WRONLY | O_APPEND |  O_CREAT |   O_TEXT   },
    {"ab"       ,O_WRONLY | O_APPEND |  O_CREAT |   O_BINARY },
    {"a+"       ,O_RDWR   | O_APPEND |  O_CREAT |   O_TEXT   },
    {"a+b"      ,O_RDWR   | O_APPEND |  O_CREAT |   O_BINARY },
    {"ab+"      ,O_RDWR   | O_APPEND |  O_CREAT |   O_BINARY },
};


/** fdopen()
Synopsis
    #include <io.h>
    FILE *_fdopen(int fd, const char *mode);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fdopen-wfdopen?view=msvc-170
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/fdopen.html
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fdopen-wfdopen?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fdopen-wfdopen?view=msvc-170#return-value
**/
FILE* _fdopen(int fd, const char* mode)
{
    CDEFILE* pCdeFile = __cdeGetIOBuffer(fd);
    char szModeNoSpace[16];
    unsigned i;

    do {
        //
        // validate mode string
        // 
        //  1. remove blanks and 't' for textmode
        if (1)
        {
            int iSrc = 0, iDst = 0;
            char c;
            while (
                c = mode[iSrc++],
                szModeNoSpace[iDst] = c,
                iDst += /* 0/1 */!(
                    (c == 0x20/* ASCII space*/) ||
                    (c == '\t') ||
                    (c == 't'/*also skip t-text, since this is not C-Specification and enabled by default*/)),
                szModeNoSpace[iDst] = '\0',
                c != 0 && iSrc < (int)sizeof(szModeNoSpace)
                );
        }

        //  2. check all possible variants
        if (1)
        {
            for (i = 0; i < sizeof(tblMode) / sizeof(tblMode[0]); i++)
            {
                int r = strcmp(szModeNoSpace, tblMode[i].pszMode);
                if (0 == r)
                    break;
            }

            if (i == sizeof(tblMode) / sizeof(tblMode[0]))      // mode string not found, 
                break;
        }

        if (1 != pCdeFile->fRsv)                                // check if file is an open file
            break;

        //
        // assign new open flags
        //
        //  TODO: check compatibility with new flags
        pCdeFile->openmode = tblMode[i].openmode;

    } while (0);

    return (FILE*)pCdeFile;
}
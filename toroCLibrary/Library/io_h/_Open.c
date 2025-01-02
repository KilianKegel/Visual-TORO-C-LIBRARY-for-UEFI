/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Open.c

Abstract:

    Implementation of the POSIX C function.
    Opens a file.

Author:

    Kilian Kegel

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
    int openmode;       /* BYTE bmDOSAccType:   000 read only
                                                001 write only
                                                010 read/write */
}tblMode[] = {
    //O_RDONLY|O_WRONLY|O_RDWR|O_APPEND|O_CREAT|O_TEXT| O_BINARY
{"r"    ,O_RDONLY                                         },
{"r+"   ,                   O_RDWR                        },
{"w"    ,          O_WRONLY                     | O_CREAT },
{"w+"   ,                   O_RDWR              | O_CREAT },
{"a"    ,          O_WRONLY         | O_APPEND  | O_CREAT },
{"a+"   ,                   O_RDWR  | O_APPEND  | O_CREAT },
};


/** open()
Synopsis

    #include <io.h>
    int open(const char* filename, int oflag, ...);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/open-wopen
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html

Parameters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/open-wopen#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/open-wopen#return-value

**/
int _open(const char* filename, int oflag, ...) {

    //TODO: add pmode support    va_list ap;
    int fd = -1;
    //int rdwroncrap/*O_RDWR O_WRONLY O_CREAT O_APPEND*/;
    int pmode = _S_IREAD | _S_IWRITE;
    char fopenmode[] = "ctrwaxb"/*backdoor signature -- case-in-sensitive */;
    FILE* fp;
    va_list ap;

    do {

        //
        // get variadic parameter for O_CREAT
        //
        va_start(ap, oflag);

        if (_O_CREAT & oflag)
            pmode = va_arg(ap, int);

        va_end(ap);

        //
        // transform O_CREAT, O_TRUNC, O_RDWR, O_WRONLY, O_APPEND, O_TEXT, O_BINARY flags 
        // to "ctrwaxb"-backdoor upper/lower case signature
        //
        fopenmode[0] = oflag & O_CREAT  ? 'C' : 'c';    // get 'C'/'c' in "ctrwaxb" 


        fopenmode[1] = oflag & O_TRUNC  ? 'T' : 't';    // get 'T'/'t' in "ctrwaxb"
        fopenmode[2] = oflag & O_RDWR   ? 'R' : 'r';    // get 'R'/'r' in "ctrwaxb"
        fopenmode[3] = oflag & O_WRONLY ? 'W' : 'w';    // get 'W'/'w' in "ctrwaxb"
        fopenmode[4] = oflag & O_APPEND ? 'A' : 'a';    // get 'A'/'a' in "ctrwaxb"
            //O_TEXT O_BINARY    fopenmode
            //    0   1           BINARY
            //    0   0           TEXT
            //    1   0           TEXT
            //    1   1           TEXT
        fopenmode[5] = (O_BINARY != (oflag & (O_TEXT | O_BINARY))) ? 'X' : 'x';    // get 'X'/'x' in "ctrwaxb"
        fopenmode[6] = (O_BINARY == (oflag & (O_TEXT | O_BINARY))) ? 'B' : 'b';    // get 'B'/'b' in "ctrwaxb"

        //
        // preprocess "ever fail" conditions
        //
        if (0 == strncmp("cTrw", fopenmode, strlen("cTrw")))
            break;

        fp = fopen(filename, fopenmode);
        //TODO: set errno

        if (NULL == fp)
            break;

        //
        // O_TEMPORARY
        //  NOTE: If pCdeFile->tmpfilename != NULL, on fclose() file is deleted
        if (O_TEMPORARY & oflag)
        {
            CDEFILE* pCdeFile = (void*)fp;
            pCdeFile->tmpfilename = malloc(sizeof('\0') + strlen(filename));
            if (NULL == pCdeFile->tmpfilename)
                abort();
            strcpy(pCdeFile->tmpfilename, filename);
        }

        fd = _fileno(fp);

    } while (0);

    return fd;

}
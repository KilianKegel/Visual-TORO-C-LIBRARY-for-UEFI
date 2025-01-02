/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Tmpfile.c

Abstract:

    Implementation of the Standard C function.
    Creates a temporary file. 

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CdeServices.h>

/** tmpfile

Synopsis
    #include <stdio.h>
    FILE *tmpfile(void);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/tmpfile?view=msvc-160&viewFallbackFrom=vs-2019
    The tmpfile function creates a temporary binary file that is different from any other
    existing file and that will automatically be removed when it is closed or at program
    termination. If the program terminates abnormally, whether an open temporary file is
    removed is implementation-defined. The file is opened for update with "wb+" mode.
Recommended practice
    It should be possible to open at least TMP_MAX temporary files during the lifetime of the
    program (this limit may be shared with tmpnam) and there should be no limit on the
    number simultaneously open other than this limit and any limit on the number of open
    files (FOPEN_MAX).
Returns
    The tmpfile function returns a pointer to the stream of the file that it created. If the file
    cannot be created, the tmpfile function returns a null pointer.
**/
FILE* tmpfile(void)
{
    char fname[L_tmpnam];
    CDEFILE* fp;

    do {

        tmpnam(fname);
        fp = (CDEFILE*)fopen(fname, "wb+");

        if (NULL == fp)
            break;

        fp->tmpfilename = malloc(1 + strlen(fname));

        if (NULL == fp->tmpfilename)
        {
            fclose((FILE*)fp);
            fp = NULL;
            break;
        }

        strcpy(fp->tmpfilename, fname);

    } while (0);

    return ((FILE*)fp);
}
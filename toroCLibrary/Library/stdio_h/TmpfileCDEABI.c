/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    TmpfileCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Creates a temporary file. 

Author:

    Kilian Kegel

--*/
//#include <stdlib.h>
#include <CdeServices.h>
//
// stdio.h
//
#define EOF    (-1)
#define WEOF ((wint_t)(0xFFFF))
#define FILE void
#define L_tmpnam   260

extern __declspec(dllimport) char* tmpnam(char* s);
extern __declspec(dllimport) FILE* fopen(const char* filename, const char* mode);
extern __declspec(dllimport) int fclose(FILE* stream);
extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);

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
static FILE* tmpfileCDEABI(void)
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

MKCDEABI(tmpfile);
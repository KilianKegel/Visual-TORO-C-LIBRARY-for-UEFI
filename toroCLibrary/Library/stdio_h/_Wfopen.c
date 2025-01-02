/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Wfopen.c

Abstract:

    Microsoft C Library specific function _wfopen()
    Opens a file.

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
//
// stdio.h
//
#define FILE void
#include <stdlib.h>
#include <string.h>
#include <CdeServices.h>

extern void* __cdeGetIOBuffer(unsigned i);
extern FILE* fopen(const char* filename, const char* mode);

/** fopen
Synopsis

    #include <stdio.h>
    FILE *_wfopen(const wchar_t *filename,const wchar_t *mode);

Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-160#return-value
**/
FILE* _wfopen(const wchar_t* wcsFileName, const wchar_t* wcsFileMode) 
{
    FILE* fp;
    size_t lenFileName = wcslen(wcsFileName);
    size_t lenFileMode = wcslen(wcsFileMode);

    char* strFileName = malloc(sizeof("") + lenFileName);
    char* strFileMode = malloc(sizeof("") + lenFileMode);

    wcstombs(strFileName, wcsFileName, sizeof("") + lenFileName);
    wcstombs(strFileMode, wcsFileMode, sizeof("") + lenFileMode);

    fp = fopen(strFileName, strFileMode);

    free(strFileName);
    free(strFileMode);


    return fp;
}
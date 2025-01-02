/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    GetcCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Read a character from a stream.


Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

__declspec(dllimport) extern int fgetc(FILE* stream);

/** 
Synopsis

    #include <stdio.h>
    int getc(FILE *stream);

Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetc-fgetwc?view=msvc-160&viewFallbackFrom=vs-2019
    Getc() is same as fgetc().
    
Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetc-fgetwc?view=msvc-160#return-value

**/
static int getcCDEABI(FILE * stream) 
{
    return fgetc(stream);
}

MKCDEABI(getc);
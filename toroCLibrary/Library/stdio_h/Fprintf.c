/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fprintf.c

Abstract:

    Implementation of the Standard C function.
    Print formatted data to a stream.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdarg.h>

/**
Synopsis

    #include <stdio.h>
    int fprintf(FILE *stream, const char *format [,   argument ]...);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fprintf-fprintf-l-fwprintf-fwprintf-l?view=msvc-160&viewFallbackFrom=vs-2019

Parameters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fprintf-fprintf-l-fwprintf-fwprintf-l?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fprintf-fprintf-l-fwprintf-fwprintf-l?view=msvc-160#return-value

**/
int fprintf(FILE* const stream, char const* const pszFormat, ...)
{
    va_list ap;
    int nRet = 0;
        
        va_start(ap, pszFormat);

        nRet = vfprintf(stream, pszFormat, ap);

        va_end(ap);


    return nRet;
}
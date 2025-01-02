/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fprintf.c

Abstract:

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    FprintfCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Print formatted data to a stream.

Author:

    Kilian Kegel

--*/
#include <stdarg.h>
#include <CdeServices.h>

#define FILE void
extern __declspec(dllimport) int vfprintf(FILE* stream, const char* pszFormat, va_list ap);
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
static int fprintfCDEABI(FILE* stream, const char* pszFormat, ...) 
{
    va_list ap;
    int nRet = 0;
        
        va_start(ap, pszFormat);

        nRet = vfprintf(stream, pszFormat, ap);

        va_end(ap);


    return nRet;
}

MKCDEABI(fprintf);

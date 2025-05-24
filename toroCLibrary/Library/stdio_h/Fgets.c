/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fgets.c

Abstract:

    Implementation of the Standard C function.
    Get a string from a stream.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void

extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
extern size_t fread(const void* ptr, size_t size, size_t nelem, FILE* stream);

/** fgets
Synopsis
    #include <stdio.h>
    char *fgets(char * restrict s, int n,FILE * restrict stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=308
    The fgets function reads at most one less than the number of characters specified by
    n from  the  stream  pointed  to  by stream into  the  array  pointed  to  by s.
    No  additional characters  are  read  after  a  new-line  character  (which  is  retained)
    or  after  end-of-file.  A null character is written immediately after the last character
    read into the array.
Returns
    The fgets function  returns s if  successful.   If  end-of-file  is  encountered  and  no
    characters have been read into the array, the contents of the array remain unchanged and a
    null pointer is returned.  If a read error occurs during the operation, the array contents are
    indeterminate and a null pointer is returned.

    @param[in] char * s
    @param[in] int  n
    @param[in] FILE *stream

    @retval char * s on success

    @retval NULL on error.

**/
char* fgets(char* s, int n, FILE* stream) {

    char c;
    int i = 0;

    if (NULL == stream
        || NULL == s
        || 0 > n) {
        //(*pinvalid_parameter_handler)(L"\"NULL == stream || NULL == s || 0 > n\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
        (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);
    }
    else
        do {

            for (i = 0, c = 0; i < (n - 1) && c != '\n'; i++) {

                if (1 != fread(&c, 1, 1, stream))
                    break;

                s[i] = c;
            }

            if (i != 0)
                s[i] = '\0';

        } while (0);

    return i == 0 ? NULL : s;
}
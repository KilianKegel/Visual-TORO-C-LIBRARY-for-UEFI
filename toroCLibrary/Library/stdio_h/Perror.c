/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Perror.c

Abstract:

    Implementation of the Standard C function.
    Print an error message.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <errno.h>

extern char* strerror(int errnum);

/** perror()

Synopsis
    #include <stdio.h>
    void perror(const char *s);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/perror-wperror?view=msvc-160&viewFallbackFrom=vs-2019
    The perror function maps the error number in the integer expression errno to an
    error message. It writes a sequence of characters to the standard error stream thus: first
    (if s is not a null pointer and the character pointed to by s is not the null character), the
    string pointed to by s followed by a colon (:) and a space; then an appropriate error
    message string followed by a new-line character. The contents of the error message
    strings are the same as those returned by the strerror function with argument errno.
Returns
    The perror function returns no value.

    @param[in] const char  *szString

    @retval void

**/
void perror(const char* string) {

    fprintf(stderr, "%s%s%s\n", NULL == string ? "" : string, NULL == string ? "" : ": ", strerror(errno));
}

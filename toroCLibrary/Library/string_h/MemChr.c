/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    MemChr.c

Abstract:

    Implementation of the Standard C function.
    Find characters in a buffer.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**
Synopsis
    #include <string.h>
    void *memchr(const void *s, int c, size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/memchr-wmemchr?view=msvc-160
    The memchr function locates the first occurrence of c (converted to an unsigned
    char) in the initial n characters (each interpreted as unsigned char) of the object
    pointed to by s.
Returns
    The memchr function returns a pointer to the located character, or a null pointer if the
    character does not occur in the object.
**/
void* memchr(const void* str, int c, size_t n) {
    unsigned char u = (unsigned char)c;
    unsigned char* pRet = (unsigned char*)str;

    while (n) {
        if (*pRet == u)
            break;
        n--;
        pRet++;
    }

    return n == 0 ? NULL : pRet;
}
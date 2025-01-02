/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Strxfrm.c

Abstract:

    Implementation of the Standard C function.
    Transform a string based on locale-specific information.

Author:

    Kilian Kegel

--*/

#include <string.h>
#include <limits.h>
#include <errno.h>

extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
/**
Synopsis
    #include <string.h>
    size_t strxfrm(char * restrict s1,const char * restrict s2,size_t n);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strxfrm-wcsxfrm-strxfrm-l-wcsxfrm-l?view=msvc-170&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=341
    The strxfrm function transforms the string pointed to by s2 and places the resulting
    string into the array pointed to by s1. The transformation is such that if the strcmp
    function is applied to two transformed strings, it returns a value greater than, equal to, or
    less than zero, corresponding to the result of the strcoll function applied to the same
    two original strings. No more than n characters are placed into the resulting array
    pointed to by s1, including the terminating null character. If n is zero, s1 is permitted to
    be a null pointer . If copying tak es place between objects that o verlap, the beha vior is
    undefined.
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strxfrm-wcsxfrm-strxfrm-l-wcsxfrm-l?view=msvc-170#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strxfrm-wcsxfrm-strxfrm-l-wcsxfrm-l?view=msvc-170#return-value
**/
size_t strxfrm(char* pszDst, const char* pszSrc, size_t n) {

    size_t nRet = INT_MAX;
    do
    {
        if ((n >= INT_MAX) || (pszDst == NULL) || (pszSrc == NULL))
        {
            errno = EINVAL;
            //(*pinvalid_parameter_handler)(L"\"NULL pointer assignment\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
            (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);
            break;
        }
        nRet =  strlen(strncpy(pszDst, pszSrc, n));

    } while (0);

    return nRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    TmpnamCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Generate names you can use to create temporary files.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern __declspec(dllimport) char* _ltoa(long value, char* str, int base);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) char* strchr(const char* str, int c);
extern __declspec(dllimport) char* strcat(char* s1, const char* s2);

/** tmpnam

Synopsis
    #include <stdio.h>
    char *tmpnam(char *s);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/tempnam-wtempnam-tmpnam-wtmpnam?view=msvc-160&viewFallbackFrom=vs-2019
    The tmpnam function generates a string that is a valid file name and that is not the same
    as the name of an existing file. The function is potentially capable of generating
    TMP_MAX different strings, but any or all of them may already be in use by existing files
    and thus not be suitable return values.
**/
static char* tmpnamCDEABI(char* s)
{

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char ext[16] = { "." };
    long l = (long)(0xFFFFFFF & pCdeAppIf->pCdeServices->pGetTsc(pCdeAppIf));


    pCdeAppIf->szTmpBuf[0] = 's';                   // MSFT names begin with "s"
    _ltoa(pCdeAppIf->lTmpNamNum, &ext[1], 36);      // generate number extension

    if (0 == pCdeAppIf->lTmpNamNum++)
    {
        _ltoa(l, &pCdeAppIf->szTmpBuf[1], 36);      // append number name after s
    }
    else
    {
        *(strchr(pCdeAppIf->szTmpBuf, '.')) = '\0'; // kill "."
    }

    strcat(pCdeAppIf->szTmpBuf, ext);               // append extension

    if (NULL != s)
    {
        strcpy(s, pCdeAppIf->szTmpBuf);
    }

    return NULL == s ? &pCdeAppIf->szTmpBuf[0] : s;

}

MKCDEABI(tmpnam);
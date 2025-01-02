/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeSplitSearchNameExt2Upcase.c

Abstract:

    CDE specific helper function to split "filename.ext" into upcase "FILENAME""EXT"

Author:

    Kilian Kegel

--*/
#undef NMOFINE
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**

Synopsis

    char* __cdeSplitSearchNameExt2Upcase(const char* pstr, char** ppStrFULL, char** ppStrEXT)

Description

    split "filename.ext" into upcase "FILENAME""EXT"

Parameter

    const char*  pStr       : IN    input searchstring
    const char** ppStrFULL  : OUT   pointer to EXTENSION
    const char** ppStrEXT   : OUT   pointer to EXTENSION

Returns

    NULL on comparison failure
    otherwise *pStr

**/
char* __cdeSplitSearchNameExt2Upcase(const char* pstr, char** ppStrFULL, char** ppStrEXT)
{
    size_t sizeStrFULL = strlen(pstr);
    char* pStrFULL = malloc(sizeof("") + sizeStrFULL)   /* full search pattern converted to UPCASE  */;
    char* pStrEXT = NULL                        /* search pattern extension e.g. "COM" or "C?M      */;
    char c;
    int i = 0;

    if (NULL == pStrFULL)
        abort();

    while ( /* copy to UPCASE */
        c = pstr[i],
        c = (char)toupper(c),
        pStrFULL[i] = c,
        pStrEXT = '.' == c ? &pStrFULL[i] : pStrEXT,    /*  memorize latest '.'  */
        i++,
        '\0' != c
        );

    if (NULL != pStrEXT)                // if '.' found
    {
        strcpy(pStrEXT++, "");          // terminate file forename and update pointer to beginning of "EXT"
        if (0 == strlen(pStrEXT))       // check empty ".EXT" --> "*."
            pStrEXT = NULL;             // empty EXT --> set NULL
    }

    if (NULL != ppStrFULL)
        *ppStrFULL = pStrFULL;

    if (NULL != ppStrEXT)
        *ppStrEXT = pStrEXT;

    return pStrFULL;
}
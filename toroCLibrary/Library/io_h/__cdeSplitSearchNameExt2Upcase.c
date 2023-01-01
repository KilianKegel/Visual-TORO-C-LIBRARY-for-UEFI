/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeSplitSearchNameExt2Upcase.c

Abstract:

    CDE specific helper function to split "filename.ext" into upcase "FILENAME""EXT"

Author:

    Kilian Kegel

--*/
#undef NMOFINE
#define MOFINE_CONFIG       MOFINE_STDOUT  | MOFINE_NDRIVER | MOFINE_NFILE |/* MOFINE_NLINE | /*MOFINE_NFUNCTION | */MOFINE_NCLOCK /*| MOFINE_NCLASS | MOFINE_RAWFORMAT */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <CdeServices.h>

/**

Synopsis
    
    char* __cdeSplitSearchNameExt2Upcase(const char* pstr, char** ppStrFULL, char** ppStrEXT)

Description

    split "filename.ext" into upcase "FILENAME""EXT"

Parameter

    const char*  pStr       : IN    input searchstring
    const char** ppStrFULL  : OUT   pointer to EXTENTION
    const char** ppStrEXT   : OUT   pointer to EXTENTION

Returns

    NULL on comparison failure
    otherwise *pStr

**/
char* __cdeSplitSearchNameExt2Upcase(const char* pstr, char** ppStrFULL, char** ppStrEXT)
{
    size_t sizeStrFULL = strlen(pstr);
    char* pStrFULL = malloc(sizeof("") + sizeStrFULL)   /* full search pattern converted to UPCASE  */;
    char* pPatNAME = pStrFULL                   /* search pattern name e.g. "COMMAND" or "CO*AN?    */;
    char* pStrEXT = NULL                        /* search pattern extention e.g. "COM" or "C?M      */;
    char c;
    int i = 0;

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
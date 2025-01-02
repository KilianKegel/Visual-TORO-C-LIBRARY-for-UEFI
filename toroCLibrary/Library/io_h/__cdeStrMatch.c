/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeStrMatch.c

Abstract:

    CDE internal string compare applying '*' and '?' wildcard rules

Author:

    Kilian Kegel

--*/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <CdeServices.h>

/**

Synopsis

    char* __cdeStrMatch(const char* pStr, const char* pPat);

Description

    Compares two strings applying '*' and '?' wildcard rules

Parameter

    const char* pStr
    const size_t lenStr    length of string
    const char* pPat

Returns

    NULL on comparison failure
    otherwise *pStr

**/
char* __cdeStrMatch(const char* pStr, const size_t lenStr, const char* pPat)
{
    char* pRet = NULL;

    if (1) do
    {
        size_t idxPat = 0/*index pattern*/, idxStr = 0/*index string == filename/fileext*/;
        size_t subpatlen;
        bool fmatch = true;

        while (fmatch == true && '\0' != pPat[idxPat] /* && '\0' != pStr[idxStr]*/)
        {
            if ((char)'*' == pPat[idxPat])
            {
                char cPatNext = 0;
                char* pStrNext = NULL;
                ptrdiff_t diff = 0;

                subpatlen = strspn(&pPat[idxPat], "*?");    // string span NOTE: IF '*' ALSO INCLUDE SUCCESSIVE '?' to be ignored
                cPatNext = pPat[idxPat + subpatlen];
                //pStrNext = strchr(&pStr[idxStr], cPatNext); // search string for cPatNext
                if (1)  // do case-in-sensitive STRCHR()
                {
                    char UpDownCase[3] = { (char)toupper(cPatNext), (char)tolower(cPatNext),'\0' };

                    if ('\0' == cPatNext)
                        pStrNext = &((char*)/*cast due "warning C4090: '=' : different 'const' qualifiers"*/pStr)[strlen(pStr)];
                    else
                        pStrNext = strpbrk(&pStr[idxStr], &UpDownCase[0]);
                }
                if (NULL != pStrNext)
                    diff = pStrNext - &pStr[idxStr];
                idxStr += diff;
            }
            else if ((char)'?' == pPat[idxPat])            
            {
                size_t len = 0;

                subpatlen = strspn(&pPat[idxPat], "?");     // string span
                len = strlen(&pStr[idxStr]);
                idxStr += min(subpatlen, len);
                fmatch = true;
            }
            else    /* non '?'/'*' */
            {
                subpatlen = strcspn(&pPat[idxPat], "*?");   // string complementary span
                fmatch = 0 == _strnicmp(&pPat[idxPat], &pStr[idxStr], subpatlen);
                idxStr += subpatlen;
            }
            idxPat += subpatlen;
        }
        
        if(true == fmatch)
            if ('\0' == pPat[idxPat])
                if('\0' == pStr[idxStr] || idxStr == lenStr)
                    pRet = (void*)pStr;
    } while (0);

    return pRet;
}

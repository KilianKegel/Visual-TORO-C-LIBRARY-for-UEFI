/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeWcsStrTok.c

Abstract:

    CDE internal worker function for all of the str/wcs--tok()-family

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern char* strpbrk(const char* pszStr, const char* pszSet);
extern wchar_t* wcspbrk(const wchar_t* pszStr, const wchar_t* pszSet);
extern size_t 	strspn(const char* pszStr, const char* pszSet);
extern size_t 	wcsspn(const wchar_t* pszStr, const wchar_t* pszSet);
unsigned int _strlen(const char* pszBuffer);
unsigned int _wcslen(const short* pszBuffer);


void* _cdeWcsStrTok(CDE_APP_IF* pCdeAppIf, IN void* pvoidStr, IN const void* pszSet, IN OUT void** ppLast, IN ROMPARM_WCSSTRTOK* pParm)
{
    void* pNew;
    char* pszStr = pvoidStr, * p;
    unsigned int (*pwcsstrlen)(const void* pszBuffer) = pParm->fWide ? (unsigned int (*)(const void*)) & _wcslen : (unsigned int (*)(const void*)) & _strlen;

    do {

        if (pszStr == NULL) {                                   // continue with previous string/wide character string...
            pszStr = *ppLast;                                   // ... yes
            if (pszStr == NULL)                                 // or have we reached the end?
                break;;                                         // ... yes
        }

        //		(char*)pszStr += (1 + pParm->fWide) * (*pwcsstrspn)(pszStr, pszSet);
        if (1) {
            p = pCdeAppIf->pCdeServices->pWcsStrPbrkSpn(pParm->fWide ? WID + INV : INV, (void*)pszStr, (void*)pszSet); // strspn() / wcsspn()

            if (NULL != p) {
                (char*)pszStr += p - pszStr;
            }
        }

        //	    pNew = (*pwcsstrpbrk)(pszStr, pszSet);                                                          // find end of substring

        pNew = pCdeAppIf->pCdeServices->pWcsStrPbrkSpn(pParm->fWide ? WID : 0, (void*)pszStr, (void*)pszSet);   // find end of substring
                                                                                                                // find end of substring
        if (pParm->fWide) {                                                                                     // find end of substring
            if ('\0' == *((short*)pNew))                                                                        // find end of substring
                pNew = NULL;                                                                                    // find end of substring
        }                                                                                                       // find end of substring
        else {                                                                                                  // find end of substring
            if ('\0' == *((char*)pNew))                                                                         // find end of substring
                pNew = NULL;                                                                                    // find end of substring
        }

        if (pNew) {                                             // if found...
            if (pParm->fWide)                                   // terminate string/wide character string
                *((short*)pNew) = '\0';
            else
                *((char*)pNew) = '\0';
            *(char**)ppLast = (char*)pNew + (1 + pParm->fWide);
        }
        else {
            *ppLast = NULL;                                     // we have reasched the end
        }

    } while (0);

    return pszStr ? ((*pwcsstrlen)(pszStr) ? pszStr : NULL) : NULL;	//return NULL for ""
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _findnext.c

Abstract:

    Microsoft C Library specific function _findnext()

TODO:   1. int _findnext64i32(intptr_t handle,struct _finddata64i32_t *fileinfo);
        2. set errno -> https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findnext-functions?view=msvc-160#return-value
        3. set errno
            EINVAL  Invalid parameter : fileinfo was NULL.Or, the operating system returned an unexpected error.
            ENOENT  No more matching files could be found.
            ENOMEM  Not enough memory or the file name's length exceeded MAX_PATH.
Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <io.h>
#include <CdeServices.h>
#include <cde.h>
//

#include "__cdeFindFirst.h"

#define _A_CDE_EXTENSION_DONE 0x80 // DONE FLAG -- File attribute constants extension from IO.H

extern char* __cdeSplitSearchNameExt2Upcase(const char* pstr, char** ppStrFULL, char** ppStrEXT);
extern char* __cdeStrMatch(const char* pStr, const size_t lenName, const char* pPat);

/** _findnext()
*
Synopsis

    #include <io.h>
    int _findnext64i32(intptr_t handle,struct _finddata64i32_t *fileinfo);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findnext-functions?view=msvc-160

Paramters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findnext-functions?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findnext-functions?view=msvc-160#return-value

**/
int _findnext64i32(intptr_t hFile, struct _finddata64i32_t* pFindData)
{
    int nRet = -1;  //assume error
    CDEFINDFIRSTNEXT* pcdeFindFirstNextData = (CDEFINDFIRSTNEXT*)hFile;
    CDEFILEINFO* pCdeFileInfo;
    size_t lenName = (size_t)-1LL;

    bool fPatEXTLess = NULL == pcdeFindFirstNextData->pstrSearchPatEXT;   // search pattern EXT less
    bool fFilEXTLess = false;   // file pattern EXT less

    do {
        for (
            /* clause-1 */
            pCdeFileInfo = pcdeFindFirstNextData->pCdeFileInfo
            ;   /* expression-2 */
            -1LL != pCdeFileInfo->time_write
            ;   /* expression-3 */
            pCdeFileInfo->attrib |= _A_CDE_EXTENSION_DONE   /* mark entry done      */,
            pCdeFileInfo = (void*)((char*)&pCdeFileInfo[0]  /* update to next entry */
                + sizeof(CDEFILEINFO)
                + strlen(pCdeFileInfo->strFileName)
                + sizeof((char)'\0'))
            )
        {

            if (_A_CDE_EXTENSION_DONE & pCdeFileInfo->attrib)
                continue;
            
            //
            // check if there is ".EXT", that means file forename length is != 0xFFFFFFFF (UINT64_MAX)
            //
            if (1)
            {
                /*
                    files:
                    "file w/o ext"
                        excluded temporarily: "."
                        excluded temporarily: ".."

                    matching EXT pattern:
                    NULL == pcdeFindFirstNextData->pstrSearchPatEXT
                    "*"  == pcdeFindFirstNextData->pstrSearchPatEXT
                    "?"  == pcdeFindFirstNextData->pstrSearchPatEXT
                        
                                            COMMAND LINE                 RESULTS IN
                    EXTless only                "*."        SearchPatNAM: "*" SearchPatEXT: "(null)"
                    EXTless + matching EXT      "*.?"       SearchPatNAM: "*" SearchPatEXT: "?"
                    EXTless + matching EXT      "*.*"       SearchPatNAM: "*" SearchPatEXT: "*"

                */
                //
                // detect length of strFileName until ".EXT"
                //
                if (strlen(pCdeFileInfo->strFileName) != strspn(pCdeFileInfo->strFileName, "."))    // skip "." and ".."
                {
                    char* pDot = strrchr(pCdeFileInfo->strFileName, '.');

                    if (pDot)
                        lenName = pDot - pCdeFileInfo->strFileName;
                    else
                        lenName = (size_t)-1LL;
                }
                else
                    lenName = (size_t)-1LL;    // doesn't include ".EXT". ".EXT" not existant
                
                fFilEXTLess = (size_t)-1LL == lenName;
                //printf("fFilEXTLess %d, fPatEXTLess %d\n", fFilEXTLess, fPatEXTLess);
            }

            //
            // check match for filenNAME/SearchPatNAME and filenEXT/SearchPatEXT
            //
            if (    false == fFilEXTLess
                &&  false == fPatEXTLess)                       // if ".EXT" present, check ".EXT" first
            {
                if (NULL == __cdeStrMatch(
                    &pCdeFileInfo->strFileName[lenName + 1],
                    (size_t)-1LL,                         // always unlimited until '\0'
                    pcdeFindFirstNextData->pstrSearchPatEXT
                ))
                    continue;
            }

            if (true == fFilEXTLess)
                if (false == fPatEXTLess)
                    if ('*' != *pcdeFindFirstNextData->pstrSearchPatEXT)
                        continue;

            if (false == fFilEXTLess
                && true == fPatEXTLess)
                continue;

            if (NULL != __cdeStrMatch(
                pCdeFileInfo->strFileName,
                lenName,
                pcdeFindFirstNextData->pstrSearchPatNAME
            )
                )
            {
                pFindData->attrib = pCdeFileInfo->attrib;
                pFindData->size = pCdeFileInfo->attrib & _A_SUBDIR ? 0 : (_fsize_t)pCdeFileInfo->size;
                pFindData->time_write = pCdeFileInfo->time_write;
                pFindData->time_access = -1LL;
                pFindData->time_create = -1LL;
                strcpy(pFindData->name, pCdeFileInfo->strFileName);
                pCdeFileInfo->attrib |= _A_CDE_EXTENSION_DONE;  // mark entry done
                nRet = 0;                                       // return "found"
                break;

            }
        }

    } while (0);
    return nRet;
}
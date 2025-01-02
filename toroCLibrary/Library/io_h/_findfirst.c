/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _findfirst.c

Abstract:

    Microsoft C Library specific function _findfirst()

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <io.h>

#include <CdeServices.h>

#include "__cdeFindFirst.h"

#define ELC(x) (sizeof(x)/sizeof(x[0]))  // element count
#define EOS '\0'
#define EOSSIZE 1/*sizeof("");*/

#define _A_CDE_EXTENSION_DONE 0x80 // DONE FLAG -- File attribute constants extension from IO.H

extern char* __cdeSplitSearchNameExt2Upcase(const char* pstr, char** ppStrFULL, char** ppStrEXT);

/** _findfirst()
* 
Synopsis

    #include <io.h>
    intptr_t _findfirst64i32(const char *filespec, struct _finddata64i32_t *fileinfo);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findfirst-functions?view=msvc-160

Paramters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findfirst-functions?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/findfirst-functions?view=msvc-160#return-value

**/
intptr_t _findfirst64i32(char const* pstrFileName, struct _finddata64i32_t* pFindData)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    intptr_t nRet = -1LL, hFile = 0;
    int nCntOfAll = 0;

    char* pstrSearchPat = NULL;
    //
    // malloc()'ed pointers to be freed at _findfirst return , included in freelist[] are initialized with NULL
    //
    char* pstrSearchAllBuffer = NULL;
    char* pstrSearchPatExt = NULL;
    char* pstrUpperDir = (0 == strcmp(pstrFileName, ".") || 0 == strcmp(pstrFileName, "..")) ? malloc(CDE_FILESYSNAME_SIZE_MAX) : NULL;  // if "."/".." else NULL
    CDEFINDFIRSTNEXT* pcdeFindFirstNextData = NULL;

    void* freelist[] = { pstrSearchAllBuffer,pstrSearchPatExt,pstrUpperDir };

    //
    // get upper directory directory "." / ".."
    // 
    if (NULL != pstrUpperDir)
    {
        pCdeAppIf->pCdeServices->pGetDrvCwd(pCdeAppIf, pstrUpperDir);

        if (0 == strcmp(pstrFileName, ".."))
        {
            char* pBSlash = strrchr(pstrUpperDir, '\\');

            if (NULL != pBSlash)
                *pBSlash = '\0';
        }
        else {//0 == strcmp(pstrFileName, ".")
            //default
        }
    }

    do
    {
        //
        // pre-process pstrFileName, replace search pattern by '*' to get all
        //
        if (1)
        {
            // consider:
            // -1) "." / ".." -> pstrUpperDir != NULL
            //  0) searchpat
            //  1) a:searchpat              (drive relative w/o back slash)
            //  2) a:bcd\searchpat          (drive relative w/  back slash)
            //  3) a:\bcd\searchpat         (drive absolute)
            //  4) a:\bcd\..\efg\searchpat
            //  5) \hij\searchpat
            //  6) \klm\..\nop\searchpat
            //  7) .\qrs\searchpat
            //  8) ..\tuv\searchpat
            //  9) \\network\hij\searchpat
            const char* pstrFileNameOrCWD = NULL == pstrUpperDir ? pstrFileName : pstrUpperDir;
            char* pBSlashRight = strrchr(pstrFileNameOrCWD, '\\');   // check if back slash (bslash) '\\' is contained before search patter
            char* pColon  = strchr(pstrFileNameOrCWD, ':');          // check if colon ':' is contained before search patter
            ptrdiff_t idxBSlash = -1;                           // separator index == offset of most right '\\'. if 0 we have somthing like "\dir\?"
            ptrdiff_t idxColon = -1;                            // index == offset of most left (and the only) ':'. This indicates there is a drive name
            int pathtype = 0;
            size_t nCpy = 0;
            CDEFILEINFO* pCdeFileInfo = NULL;

            if ((NULL != pBSlashRight))
                idxBSlash = pBSlashRight - pstrFileNameOrCWD;
            if ((NULL != pColon))
                idxColon = pColon - pstrFileNameOrCWD;
            #define UPPERDIR   -1   /*  ONLY "???" or "*.txt" w/o path/drive relation   */
            #define PATTERNONLY 0   /*  ONLY "???" or "*.txt" w/o path/drive relation   */
            #define BSLASH      1   /*  back slash present                              */
            #define COLON       2   /*  : == drive present                              */
            #define BSLASHAT0   4   /*  back slash a  offset 0 e.g. "\dir\*.txt         */

            if (NULL != pstrUpperDir)
            {
                pathtype = UPPERDIR;
            }
            else
            {
                pathtype = PATTERNONLY;                         // 000  "pat" only -> search in current directory
                pathtype += BSLASH * (NULL != pBSlashRight);    // 001  index of '\' is != 0 e.g. "..\dir\pat" or ".\pat" or "\dir\pat" or "\pat"
                pathtype += COLON * (NULL != pColon);           // 010  e.g. A:
                pathtype += BSLASHAT0 * (0 == idxBSlash);       // 100  index of '\' is 0 e.g. "\dir\pat" or "\pat"
            }

            switch (pathtype)
            {
            case PATTERNONLY        :                       // "pat"
                nCpy = 0;
                pstrSearchPat = (char*)&pstrFileNameOrCWD[nCpy];
                break;

            case BSLASH             :                       // index of '\' is != 0 e.g. "..\dir\pat" or ".\pat" BUT NOT "\pat"
            case BSLASH + COLON:                            // index of '\' is != 0 e.g. "x:\dir\pat" or "x:dir\pat" or "x:\pat"
            case BSLASH + BSLASHAT0:                        // index of '\' is == 0 e.g. "\dir\pat" or "\pat"
                nCpy = idxBSlash + 1;
                pstrSearchPat = (char*)&pstrFileNameOrCWD[nCpy];
                break;
            case COLON:                                     // e.g. "x:pat"
                nCpy = idxColon + 1;
                pstrSearchPat = (char*)&pstrFileNameOrCWD[nCpy];
                break;
            case UPPERDIR:
                nCpy = idxBSlash + 1;
                pstrSearchPat = (char*)&pstrFileNameOrCWD[nCpy];
            }

            pstrSearchAllBuffer = malloc(320/* additional scratch space*/ + nCpy + sizeof("*"));
            if (NULL == pstrSearchAllBuffer)
                abort();
            strncpy(pstrSearchAllBuffer, pstrFileNameOrCWD, nCpy);
            pstrSearchAllBuffer[nCpy] = '\0';
            strcat(pstrSearchAllBuffer, "*");

            //
            // duplicate pstrSearchPat and increase size to be r/w and extensible for ".*"
            //
            pstrSearchPatExt = malloc(sizeof(".*")/* including '\0' */ + strlen(pstrSearchPat));
            if (NULL == pstrSearchPatExt)
                abort();
            strcpy(pstrSearchPatExt, pstrSearchPat);

           
            // adjust search pattern to real world needs
            //  e.g. "*"    -> "*.*"
            //       "xy*"  -> "xy*.*"
            if (NULL == pstrUpperDir)
            {
                char *pDot = strrchr(pstrSearchPatExt, '.');
                size_t eos = strlen(pstrSearchPatExt);

                if(     NULL == pDot                        /* if NOT contains '.'              */
                    && '*'  == pstrSearchPatExt[eos - 1]    /* and '*' is the last character    */
                    )
                {
                    strcat(pstrSearchPatExt, ".*");
                }
            }

            //
            // read the entire directory content
            //
            pCdeFileInfo = pCdeAppIf->pCdeServices->pFfindall(
                pCdeAppIf,
                pstrSearchAllBuffer,
                &nCntOfAll
            );

            ///////////////////////////////////////////////////////////////////////////
            // TODO: CHECK RETURN VALUE
            ///////////////////////////////////////////////////////////////////////////


            if (    NULL != pCdeFileInfo
                && -1LL != pCdeFileInfo->time_write/*end marker*/)
            {
                //pstrSearchPatExt   = malloc(sizeof((char)'\0') + strlen(pstrSearchPat));
                
                //
                // allocate the "handle" that include the file list, file count and pattern NAME + EXT
                //
                pcdeFindFirstNextData = malloc(sizeof(CDEFINDFIRSTNEXT));
                if (NULL == pcdeFindFirstNextData)
                    abort();

                pcdeFindFirstNextData->nCountOfAll = nCntOfAll;
                pcdeFindFirstNextData->pCdeFileInfo = pCdeFileInfo;
                
                //
                // split the search pattern "pattern.ext" into "PATTERN""EXT"
                //
                __cdeSplitSearchNameExt2Upcase(
                    //strcpy(/*pstrSearchPatExt*/malloc(sizeof((char)'\0') + strlen(pstrSearchPat)), 
                    //    pstrSearchPat),
                    pstrSearchPatExt,
                    &pcdeFindFirstNextData->pstrSearchPatNAME,
                    &pcdeFindFirstNextData->pstrSearchPatEXT
                    );

                hFile = (intptr_t)pcdeFindFirstNextData;
                
                if (0 == _findnext(hFile, pFindData))
                    nRet = hFile;
            }
        }

    } while (0);

    
    if (1)// free allocated memory
    {
        int i;
        for (i = 0; i < (int)ELC(freelist); i++)
            free(freelist[i]);
    }

    return nRet;
}

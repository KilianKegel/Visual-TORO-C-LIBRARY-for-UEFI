/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fopen.c

Abstract:

    Implementation of the Standard C function.
    Opens a file.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CdeServices.h>

extern void* __cdeGetIOBuffer(unsigned i);

/** fopen
Synopsis

    #include <stdio.h>
    FILE *fopen(const char *filename,const char *mode);

Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-160&viewFallbackFrom=vs-2019

Parameters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-160#parameters

Returns

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-160#return-value

**/
FILE* fopen(const char* filename, const char* mode) {

    char rgModeCopy[16], szModeNoSpace[16], * pc = NULL;
    const char szDelims[] = { " \tt" };
    unsigned char TODO = 1;
    CDEFILE* pCdeFile = 0;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int i = 0;;
    wchar_t* pwcsFileName = malloc(sizeof(wchar_t) * (1 + strlen(filename))); // allocate space to expand the filename to wide character

    do {/*1. dowhile(0)*/
    //
    // sanity check
    //
        if (NULL == pCdeAppIf->pCdeServices->pFopen) {
            return NULL;
        }
        //
        // ----- expand filename to wide character
        //
        i = 0;
        while ('\0' != (pwcsFileName[i] = filename[i++]))
            ;

        //
        // ----- remove spaces from mode string, remove t (text), that is not Standard C
        //
        strncpy(rgModeCopy, mode, 15);                                          //copy to rw memory (/Gy[-] separate functions for linker)
        rgModeCopy[15] = '\0';                                                  //set termination
        szModeNoSpace[0] = '\0';                                                //init space removed copy of mode
        pc = strtok((void*)&rgModeCopy[0], &szDelims[0]);
        do {
            if (NULL == pc)
                break;
            strcpy(&szModeNoSpace[strlen(szModeNoSpace)], pc);
            pc = strtok(NULL, &szDelims[0]);
        } while (1);

        //CDETRACE((CDEFINE"szModeNoSpace: %s\n",szModeNoSpace));

        if (TODO) {
            //TODO: add filename separation into drive, (relative) pathname, filename etc.
            //20160806 assumed, that filename ist filename only
            //wchar_t *pWcsCurDir,*pWcsCurDrv={L"."},*pWcsTmp;
    //
    // ----- find free CdeFile slot
    //

            for (i = 0, pCdeFile = __cdeGetIOBuffer(0)/*&_iob[0]*/; NULL != pCdeFile; pCdeFile = __cdeGetIOBuffer(++i)){
                if (FALSE == pCdeFile->fRsv) {

                    memset(pCdeFile, 0, sizeof(CDEFILE));

                    pCdeFile->fRsv = TRUE;
                    //                    pCdeFile->pwcsFileDrive = pWcsCurDrv;
                    //                    //TODO: pCdeFile[i].pwcsFilePath =
                    //                    pCdeFile->pRootProtocol = i == CDE4WIN_NA ? NULL : gCdeSystemVolumes.rgFsVolume[i].pRootProtocol;
                    break;
                }
            }

            if (i == pCdeAppIf->cIob) {
                //no free CDEFILE slot found
                //TODO: add error "errno" here
                break;/*1. dowhile(0)*/
            }

            // ----- initialize fpos bpos bidx

            pCdeFile->emufp = pCdeAppIf->pCdeServices->pFopen(pCdeAppIf, pwcsFileName, szModeNoSpace, pCdeFile);// get emulation file pointer, that is the Windows FP (CDE4WIN) or pCdeFile or NULL in error case

            if (pCdeFile->emufp == NULL)
                pCdeFile->fRsv = 0;
        }

    } while (0)/*1. dowhile(0)*/;

    free(pwcsFileName);

    return pCdeFile->emufp == NULL ? NULL : (FILE*)pCdeFile;
}
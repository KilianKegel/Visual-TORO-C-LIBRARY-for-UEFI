/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fopen.c

Abstract:

    Implementation of the Standard C function.
    Opens a file.

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
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
        // expand filename to wide character
        //
        i = 0;
        while ('\0' != (pwcsFileName[i] = filename[i]))
            i++;

        //
        // ----- remove spaces from mode string, remove t (text), that is not Standard C
        //
        strncpy(rgModeCopy, mode, 15);                                          //copy to rw memory (/Gy[-] separate functions for linker)
        rgModeCopy[15] = '\0';                                                  //set termination

        if (0 == _stricmp(rgModeCopy, "ctrwaxb"))
        {
            strcpy(szModeNoSpace, rgModeCopy);
        }
        else {
            //
            // NOTE: strtok() is not reentrant (due to internal buffer usage) and collides with external strtok() calls in conjunction with fopen(), _stat() etc.
            //       Do not use strtok() in this context, use pCdeAppIf->pCdeServices->pWcsStrTok() instead
            //
            void* pStrtokStaticInternal = NULL;                                     // scratch buffer for strtok()
            static ROMPARM_WCSSTRTOK ROMPARM = {
                /*fForceToDataSeg       */ 1 ,\
                /*fWide                 */ sizeof(char) > 1 ,\
            };
            szModeNoSpace[0] = '\0';                                                //init space removed copy of mode
            pc = (char*)pCdeAppIf->pCdeServices->pWcsStrTok(pCdeAppIf, (void*)&rgModeCopy[0], &szDelims[0], &pStrtokStaticInternal, &ROMPARM);
            do {
                if (NULL == pc)
                    break;
                strcpy(&szModeNoSpace[strlen(szModeNoSpace)], pc);
                pc = (char*)pCdeAppIf->pCdeServices->pWcsStrTok(pCdeAppIf, NULL, &szDelims[0], &pStrtokStaticInternal, &ROMPARM);
            } while (1);
        }


        if (TODO) {
            //TODO: add filename separation into drive, (relative) pathname, filename etc.
            //20160806 assumed, that filename ist filename only
            //wchar_t *pWcsCurDir,*pWcsCurDrv={L"."},*pWcsTmp;
    //
    // ----- find free CdeFile slot
    //

            for (i = 0, pCdeFile = __cdeGetIOBuffer(0)/*&_iob[0]*/; NULL != pCdeFile; pCdeFile = __cdeGetIOBuffer(++i)) {
                if (FALSE == pCdeFile->fRsv) {

                    memset(pCdeFile, 0, sizeof(CDEFILE));

                    pCdeFile->fRsv = TRUE;
                    //  pCdeFile->pwcsFileDrive = pWcsCurDrv;
                    //  //TODO: pCdeFile[i].pwcsFilePath =
                    //  pCdeFile->pRootProtocol = i == CDE4WIN_NA ? NULL : gCdeSystemVolumes.rgFsVolume[i].pRootProtocol;
                    break;
                }
            }

            if (i == pCdeAppIf->cIob) {
                //no free CDEFILE slot found
                //TODO: add error "errno" here
                break;/*1. dowhile(0)*/
            }

            if (NULL == pCdeFile)
                break;
            //
            // open the file, for POSIX open()/Microsoft _open() additionally check existance/presence of requested file
            //  NOTE:   The "existance check" is an additional step that is required for POSIX open()/Microsoft _open() support
            //          in UEFI. With that additional step it is easier to satisfy the flag matrix
            //              a) nonexisting files
            //              b) existing files r/w 
            //              c) existing files r/o
            //          The flag matrix contains all combinations of O_CREATE, O_APPEND, O_TRUNC, O_WRONLY and O_RDWR
            //
            CDEFILE CdeFileTmp, * pCdeFileTmp = memset(&CdeFileTmp, 0, sizeof(CDEFILE));
            int errnoorg = errno;                                                                   // preserve errno across existance check

            int fFileExists = NULL != (pCdeFileTmp->emufp = pCdeAppIf->pCdeServices->pFopen(        // check existance of file
                pCdeAppIf, 
                pwcsFileName, 
                "r", 
                0/*no matter 0 or 1*/, 
                pCdeFileTmp)); 

            errno = errnoorg;                                                                       // preserve errno across existance check

            if (NULL != pCdeFileTmp->emufp)
                pCdeAppIf->pCdeServices->pFclose(pCdeAppIf, pCdeFileTmp);       // instantly close file


            pCdeFile->emufp = pCdeAppIf->pCdeServices->pFopen(
                pCdeAppIf,
                pwcsFileName,
                szModeNoSpace,
                fFileExists,  /* 1 == file present, 0 == file not present*/
                pCdeFile);// get emulation file pointer, that is the Windows FP (CDE4WIN) or pCdeFile or NULL in error case

            if (pCdeFile->emufp == NULL)
                pCdeFile->fRsv = 0;

            //
            // MSFT UNDEFIND BEHAVIOUR POST PROCESSING:
            //  errno:=22 == "Invalid argument" set even on successful fopen() with the conditions below:
            if (1) 
            {
                if (NULL != pCdeFile->emufp)
                    if(     (0 == strcmp("w+", szModeNoSpace))
                        ||  (0 == strcmp("a+", szModeNoSpace) && 0 == fFileExists))
                    errno = EINVAL;/*"Invalid argument - UNDOCUMENTED IN ERRNO.H"*/
            }
        }

    } while (0)/*1. dowhile(0)*/;

    free(pwcsFileName);

    return (NULL == pCdeFile || pCdeFile->emufp == NULL) ? NULL : (FILE*)pCdeFile;
}
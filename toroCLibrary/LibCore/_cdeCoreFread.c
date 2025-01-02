/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreFread.c

Abstract:

    fread() for core mode (that means: CDE_APP_IF* pCdeAppIf is an additional parameter)

    Implementation of the Standard C function.
    Reads data from a stream.
    CTRL-Z, and text vs. binary mode is supported to be Microsoft C Library compatible

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <CdeServices.h>

extern int __cdeIsFilePointerCORE(CDE_APP_IF *pCdeAppIf, void* stream);
extern void* __cdereallocCORE(CDE_APP_IF* pCdeAppIf, void* ptr, size_t size);
extern void __cderewindCORE(CDE_APP_IF* pCdeAppIf, FILE* stream);
extern int _cdeCoreFflush(CDE_APP_IF* pCdeAppIf, FILE* stream);
/** 
Synopsis

    size_t _cdeCoreFread(CDE_APP_IF *pCdeAppIf, void *ptr, size_t size, size_t nelem, FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-160&viewFallbackFrom=vs-2019
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=313
    The function reads characters from the input stream stream and stores
    them in successive elements of the array whose first element has the
    address (char *)ptr until the function stores size*nelem characters or
    sets the end-of-file or error indicator. It returns n/size, where n is the
    number of characters it read. If n is not a multiple of size, the value
    stored in the last element is indeterminate. If the function sets the error
    indicator, the file-position indicator is indeterminate.
Paramters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-160#return-value
**/

size_t _cdeCoreFread(CDE_APP_IF* pCdeAppIf, const void* ptr, size_t size, size_t nelem, FILE* stream) {

    CDEFILE* pCdeFile = (CDEFILE*)stream;
    size_t nRet = 0, requested, provided, skipped/* #CR */;
    size_t lastnum;
    unsigned char fposoosync = TRUE; //file position out of sync
    unsigned char fLFexpected;/*line feed*/
  

    if(__cdeIsFilePointerCORE(pCdeAppIf, pCdeFile)) do
    {
        fpos_t fpos = pCdeFile->bpos + pCdeFile->bvld;                                  //memorize file position that is destroyed by a possible fwrite(NULL,EOF,0,pCdeFile)

        if (O_WRONLY == (pCdeFile->openmode & (O_RDONLY | O_WRONLY | O_RDWR))) {
            //DONT ADD ERRoR TO ERRNO
            pCdeFile->fErr = TRUE;
            break;
        }

        if (NULL == pCdeFile->Buffer) {
            pCdeFile->Buffer = __cdereallocCORE(pCdeAppIf, 0, BUFSIZ);
            pCdeFile->bsiz = BUFSIZ;
            pCdeFile->bufPosEOF = LONG_MAX;// no EOF inside the buffer, yet
            pCdeFile->fCtrlZ = FALSE;
            pCdeFile->cntSkipCtrlZChk = 0;
        }

        provided = skipped = lastnum = 0;
        requested = size * nelem;
        fLFexpected = FALSE;

        pCdeFile->fEof = pCdeFile->bidx >= pCdeFile->bufPosEOF;
        if (O_CDESTDIN == (pCdeFile->openmode & (O_CDEREDIR + O_CDESTDIN)))
            if (pCdeFile->fEof)
                __cderewindCORE(pCdeAppIf, (FILE*)pCdeFile);

        for (/* done above */; (provided + skipped) < requested && pCdeFile->fCtrlZ == FALSE && pCdeFile->bidx < pCdeFile->bufPosEOF && 0 == pCdeFile->fEof; /* do nothing */)
        {
            if ((0 == pCdeFile->bvld) || pCdeFile->bidx >= pCdeFile->bvld)
            {

                if (pCdeFile->bdirty && !pCdeFile->bclean)
                {
                    _cdeCoreFflush(pCdeAppIf, (FILE*)pCdeFile);
                }

                if (fposoosync) {
                    pCdeAppIf->pCdeServices->pFsetpos(pCdeAppIf, pCdeFile, (CDEFPOS_T*) & fpos);
                    fposoosync = FALSE;
                }

                lastnum = pCdeAppIf->pCdeServices->pFread(pCdeAppIf, pCdeFile->Buffer, pCdeFile->bsiz, pCdeFile);

                //
                // buffer EOF, keep track of buffer offset of EOF. No need do read 0 bytes anymore
                //
                pCdeFile->bufPosEOF = (size_t)pCdeFile->bsiz != lastnum ? (int)lastnum : LONG_MAX;

                pCdeFile->fEof = 0 == lastnum ? TRUE : FALSE;
                pCdeFile->bpos += lastnum;
                pCdeFile->bidx = 0;
                pCdeFile->bvld = (long)lastnum;

                fposoosync = ((int)lastnum != pCdeFile->bsiz ? TRUE : FALSE); /* force resync if e.g. last byte in the buffer was CR and unknow if next byte will be LF */

                if (0 == lastnum)
                    break;
            }

            for (/* provided = 0 , requested = size * nelem */; (provided + skipped) < requested && pCdeFile->bidx < pCdeFile->bvld && pCdeFile->fCtrlZ == FALSE; /* do nothing */)
            {

                if (TRUE == fLFexpected) {
                    fLFexpected = FALSE;
                    if ('\n' != pCdeFile->Buffer[pCdeFile->bidx]) {
                        provided++;
                        if ((provided + skipped) == requested)
                            break;
                    }
                    else {
                        skipped++;
                    }
                }

                ((char*)ptr)[provided] = pCdeFile->Buffer[pCdeFile->bidx];
                pCdeFile->bidx++;
                pCdeFile->bclean = TRUE;

                if (pCdeFile->openmode & O_TEXT) {
                    if ('\r' == ((char*)ptr)[provided]) {
                        fLFexpected = TRUE;
                        continue;
                    }

                    if (0 == pCdeFile->cntSkipCtrlZChk)
                    {
                        if (0x1A == ((char*)ptr)[provided]) {
                            pCdeFile->fCtrlZ = TRUE;
                            break;
                        }
                    }
                    else
                        pCdeFile->cntSkipCtrlZChk--;
                }
                provided++;
            }
        }

        pCdeFile->bpos -= lastnum;

        if (TRUE == fLFexpected)
            provided++;

        pCdeFile->fEof = pCdeFile->bufPosEOF == pCdeFile->bidx 
                        && provided < requested;

        nRet = provided / (size == 0 ? 1 : size/*don't divide by zero*/);

    }while (0);//if(__cdeIsFilePointerCORE(pCdeAppIf, pCdeFile)) do

    return nRet;
}
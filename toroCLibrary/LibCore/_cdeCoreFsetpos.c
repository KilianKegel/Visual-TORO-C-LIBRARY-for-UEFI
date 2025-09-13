/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreFsetpos.c

Abstract:

    fsetpos() for core mode (that means: CDE_APP_IF* pCdeAppIf is an additional parameter)

    Implementation of the Standard C function.
    Set file position indicator.

Author:

    Kilian Kegel

--*/
#include <uefi.h>
//
// stdio.h
//
#define EOF    (-1)
#define FILE void
#define BUFSIZ  512
#define SEEK_END    2
#define SEEK_SET    0
//
// errno.h
//
extern __declspec(dllimport) int* _errno(void);
#define errno (*_errno())
#define EBADF           9
#define EINVAL          22

#include <limits.h>

#include <CdeServices.h>

extern int __cdeIsFilePointerCORE(CDE_APP_IF *pCdeAppIf, void* stream);
extern int __cdeOnErrSet_errnoCORE(CDE_APP_IF* pCdeAppIf, CDE_STATUS Status, int Error);
extern int __cdeIsCdeFposType(fpos_t fpos);
extern int __cdeBiasCdeFposType(fpos_t fpos);
extern fpos_t __cdeOffsetCdeFposType(fpos_t fpos);
extern int _cdeCoreFflush(CDE_APP_IF* pCdeAppIf, FILE* stream);

/*
Synopsis
    int _cdeCoreFflush(CDE_APP_IF *pCdeAppIf, FILE *stream, const fpos_t *pos);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fsetpos?view=msvc-170
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=315
    The fsetpos function sets the mbstate_t object (if any) and file position indicator
    for the stream pointed to by stream according to the value of the object pointed to by
    pos, which shall be a value obtained from an earlier successful call to the fgetpos
    function on a stream associated with the same file. If a read or write error occurs, the
    error indicator for the stream is set and fsetpos fails.
    A successful call to the fsetpos function undoes any effects of the ungetc function
    on the stream, clears the end-of-file indicator for the stream, and then establishes the new
    parse state and position. After a successful fsetpos call, the next operation on an
    update stream may be either input or output.
Returns
    If successful, the fsetpos function returns zero; on failure, the fsetpos function
    returns nonzero and stores an implementation-defined positive value in errno.

NOTE: Parameter fpos_t* pos is handled internally as CDEFPOS_T.

*/
int _cdeCoreFsetpos(CDE_APP_IF *pCdeAppIf, FILE* stream, const fpos_t* pos)
{
    CDEFILE* pCdeFile = (CDEFILE*)stream;
    int nRet = EOF, nRetErr = EOF, *pRet = &nRet;
    
    CDEFPOS_T CdeFPos;
    CdeFPos.fpos64 = *pos;

    do {

        if (!__cdeIsFilePointerCORE(pCdeAppIf, pCdeFile))
        {
            errno = EBADF;
            break;
        }
        
        if (pCdeFile->bdirty && !pCdeFile->bclean)
        {
            _cdeCoreFflush(pCdeAppIf, stream);
        }

        if (1/*KG20220419*/)
        {
            //
            // check native erroneous negative offsets, BEFORE begin of file
            //  NOTE: Microsoft reports errno 22 / EINVAL / "Invalid argument" but truly moves filepointer to EOF
            //
            //      
            char fSetEINVAL = 0;
            
            //
            //  SEEK_SET, begin of file. Every negative offset is erroneus!
            //
            if (SEEK_SET == __cdeBiasCdeFposType(*pos))
            {
                int64_t RequestedSeekPointer = __cdeOffsetCdeFposType(*pos);

                if (0 > RequestedSeekPointer)
                    fSetEINVAL = 1;
            }

            //
            //  SEEK_END, end of file. Only negative offsets bigger (absolut value) that filesize are erroneus!
            //
            if (SEEK_END == __cdeBiasCdeFposType(*pos))
            {
                int64_t EOFPointer, RequestedSeekPointer = __cdeOffsetCdeFposType(*pos);
             
                if (0 > RequestedSeekPointer)
                {
                    //
                    // get EOF position
                    //
                    CDEFPOS_T CdeFposEOF = { .fpos64 = 0, .CdeFposBias.Bias = CDE_SEEK_BIAS_END };
                    CDEFPOS_T CdeFposCurrent;
                    CdeFposCurrent.fpos64 = pCdeFile->bpos;

                    nRet = pCdeAppIf->pCdeServices->pFsetpos(pCdeAppIf, pCdeFile, &CdeFposEOF);

                    EOFPointer = pCdeFile->bpos;

                    if (0 > (RequestedSeekPointer + EOFPointer))
                        fSetEINVAL = 1;

                    nRet = pCdeAppIf->pCdeServices->pFsetpos(pCdeAppIf, pCdeFile, &CdeFposCurrent);
                }
            }

            if(1 == fSetEINVAL)
            {
                errno = EINVAL;

                if (    CDE_SEEK_BIAS_SET == CdeFPos.CdeFposBias.Bias
                    ||  CDE_SEEK_BIAS_END == CdeFPos.CdeFposBias.Bias)
                {
                    break;                                  // don't move the file pointer
                }
                else
                {
                    CdeFPos.fpos64 = 0LL;
                    CdeFPos.CdeFposBias.Bias = CDE_SEEK_BIAS_END;

                    pRet = &nRetErr;                        // don't break, move the file pointer 
                                                            // and continue but force erroneous return
                }
            }
        }

        nRet = pCdeAppIf->pCdeServices->pFsetpos(pCdeAppIf, pCdeFile, &CdeFPos);                // move the file pointer

        // ----- reset data structure if not yet done by fflush(), bpos is set by pFsetpos();

        pCdeFile->bidx = 0;     // also clear bidx, since fgetpos == *pos = pCdeFile->bpos + pCdeFile->bidx;
        pCdeFile->bvld = 0;
        pCdeFile->bdirty = FALSE;
        pCdeFile->bclean = FALSE;
        pCdeFile->bufPosEOF = LONG_MAX;
        pCdeFile->fCtrlZ = FALSE;
        pCdeFile->cntSkipCtrlZChk = 0;
        
    } while (0);
    //TODO: Add errno
    return *pRet;
}
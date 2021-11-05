/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fsetpos.c

Abstract:

    Implementation of the Standard C function.
    Set file position indicator.

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <CdeServices.h>

extern unsigned char __cdeIsFilePointer(void* stream);
extern int __cdeOnErrSet_errno(CDE_STATUS Status, int Error);

/*
Synopsis
    #include <stdio.h>
    int fsetpos(FILE *stream, const fpos_t *pos);
Description
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
*/
int fsetpos(FILE* stream, const fpos_t* pos)
{
    CDEFILE* pCdeFile = (CDEFILE*)stream;
    int nRet = EOF;
    EFI_STATUS Status = EFI_SUCCESS;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    do {

        if (!__cdeIsFilePointer(pCdeFile)) {
            __cdeOnErrSet_errno(Status, EBADF);
            break;
        }

        if (pCdeFile->bdirty && !pCdeFile->bclean)
        {
            fflush(stream);
        }

        pCdeFile->fEof = *pos & CDE_FPOS_SEEKEND ? TRUE : FALSE;

        nRet = pCdeAppIf->pCdeServices->pFsetpos(pCdeAppIf, pCdeFile, pos);

        // ----- reset data structure if not yet done by fflush(), bpos is set by pFsetpos();

        pCdeFile->bidx = 0;     // also clear bidx, since fgetpos == *pos = pCdeFile->bpos + pCdeFile->bidx;
        pCdeFile->bvld = 0;
        pCdeFile->bdirty = FALSE;
        pCdeFile->bclean = FALSE;
        pCdeFile->bufPosEOF = LONG_MAX;

    } while (0);
    //TODO: Add errno
    return nRet;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fclose.c

Abstract:

    Implementation of the Standard C function.
    Closes a stream (fclose).

TODO:
    1. invoke invalid parameter handler on NULL FILE pointer
    2. check return value not to be a "EFI_STATUS"

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
#include <stdio.h>
#include <stdlib.h>
#include <CdeServices.h>

extern int __cdeIsFilePointer(void* stream);

/** fclose

Synopsis
    #include <stdio.h>
    int fclose(FILE *stream);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-160&viewFallbackFrom=vs-2019
    A successful call to the fclose function causes the stream pointed to by stream to be
    flushed and the associated file to be closed. Any unwritten buffered data for the stream
    are delivered to the host environment to be written to the file; any unread buffered data
    are discarded. Whether or not the call succeeds, the stream is disassociated from the file
    and any buffer set by the setbuf or setvbuf function is disassociated from the stream
    (and deallocated if it was automatically allocated).
Returns
    The fclose function returns zero if the stream was successfully closed, or EOF if any
    errors were detected.

    @param[in] FILE *stream

    @retval 0 Procedure returned successfully.

    @retval EOF Procedure returned non-successfully.

**/
int fclose(FILE* stream) {
    CDEFILE* pCdeFile = (void*)stream;
    int nRet = EOF;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    //TODO add ERRNO


    if (__cdeIsFilePointer(stream)) do {

        // NOTE:EFI_FILE_PROTOCOL.Delete() has to be called on OPEN file!!!

        if (NULL != pCdeFile->tmpfilename && SHELLIF == pCdeAppIf->DriverParm.CommParm.OSIf)    // UEFI tmpfile
        {

            nRet = pCdeAppIf->pCdeServices->pFdelete(pCdeAppIf, pCdeFile->tmpfilename, pCdeFile);

        }
        else
        {

            if (pCdeFile->bdirty && !pCdeFile->bclean)
            {
                fflush((void*)pCdeFile);    // NULL,EOF,0,stream == flush parameter
            }

            nRet = pCdeAppIf->pCdeServices->pFclose(pCdeAppIf, pCdeFile);

        }

        free(pCdeFile->Buffer);     // free the buffer

        if (NULL != pCdeFile->tmpfilename && SHELLIF != pCdeAppIf->DriverParm.CommParm.OSIf)    // non-UEFI tmpfile
        {
            remove(pCdeFile->tmpfilename);
        }

        pCdeFile->Buffer = NULL;    // mark pointer free

        pCdeFile->fRsv = FALSE;     // clear reserved flag

        pCdeFile->tmpfilename = NULL;

    } while (0);

    return nRet;
}
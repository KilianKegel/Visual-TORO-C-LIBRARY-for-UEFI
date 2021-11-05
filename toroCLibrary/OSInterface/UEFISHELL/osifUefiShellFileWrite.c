/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileWrite.c

Abstract:

    OS interface (osif) write file UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <PiPei.h>

#include <Base.h>
#include <CdeServices.h>
#include <stdio.h>

extern char _gSTDOUTMode;   /* 0 == UEFI Shell default, 1 == ASCII only */

/**

Synopsis
    #include <CdeServices.h>
    size_t _osifUefiShellFileWrite(IN CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile);
Description
    Write file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    void* ptr                   : buffer
    size_t nelem                : number of bytes
    CDEFILE* pCdeFile           : CDEFILE* file handle
Returns
    number of bytes written
    
**/
size_t _osifUefiShellFileWrite(IN CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile) {

    size_t count = 0;
    size_t BufferSize = 0;
    size_t elmsize = 1;                              // 1 16bit write, 2 8Bit write
//        size_t *pBufferSize = pCdeFile->openmode & O_CDEWCSZONLY ? &BufferSize1 : &BufferSize;
    char* pBuffer = ptr;
    void* p = ptr;
    unsigned int i;
    EFI_STATUS Status;
    static wchar_t wcBuffer[BUFSIZ + 1/*termination zero*/];/* BUFSIZ can not be changed on STDOUT/STDERR */
    unsigned char* pUni = (unsigned char*)&pCdeFile->pFileProtocol->OpenEx;

#define OPENMODE pCdeFile->openmode

    do {
        if (OPENMODE & O_CDESTDOUT || OPENMODE & O_CDESTDERR)
        {
            //
            // suppress ASCII only mode for STDOUT and STDERR
            //
            if (1 == _gSTDOUTMode/* 0 == UEFI Shell default, 1 == ASCII only */) {
                *pUni = 'T';
                OPENMODE |= O_CDEWIDTH16;
            }
            else {
                if (0 == (OPENMODE & O_CDEREDIR) /* Console is NOT redirected */) {

                    for (i = 0; i < nelem; i++)
                        wcBuffer[i] = 0xFF & pBuffer[i];

                    wcBuffer[i] = '\0';/*termination zero*/

                    elmsize = 2;
                    p = &wcBuffer[0];
                }
                else { // STDOUT/STDERR is redirected to file

                    if (0 == *pUni) {
                        *pUni = 'T';
                    }
                    else {
                        for (i = 0; i < nelem; i++)
                            wcBuffer[i] = 0xFF & pBuffer[i];

                        elmsize = 2;
                        p = &wcBuffer[0];
                    }
                }
            }
        }
        BufferSize = nelem * elmsize;
        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Write(pCdeFile->pFileProtocol, &BufferSize, p));
        count = BufferSize / elmsize;

        *pUni = 'T' == *pUni ? 0 : *pUni;

    } while (0);

    return  EFI_SUCCESS == Status ? (size_t)count : 0;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileWrite.c

Abstract:

    OS interface (osif) write file UEFI Shell

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
#define OS_EFI
#include <uefi.h>
#include <Base.h>
#include <CdeServices.h>
#include <stdio.h>
#include <stdint.h>

//EFI_SYSTEM_TABLE* _cdegST;
//extern char trcen;
//short wcsbuf[512];
//extern int swprintf(short* pszDest, size_t dwCount, const short* pszFormat, ...);
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
            if (1 == pCdeAppIf->STDOUT816BitMode/* 0 == UEFI Shell default, 1 == ASCII only */) {
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

        if (1/*KG20220418 gap of non-initialized disk space*/)
        {
            //
            // UEFI BUG: file positioning bug, if data written behind EOF, data range between old EOF and 
            // new data contains medium data / garbage, instead of 0
            //
//            if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d\r\n", __FUNCTION__, __LINE__), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);
            if (0 != pCdeFile->gapsize)
            {
                static char zBuf64k[64 * 1024];                                                                 // reserve static 64k buffer initialized to 0 in .bss
                EFI_STATUS Status1, Status2;
                int64_t sGapSize = (long long)pCdeFile->gapsize;                                                // convert size to signed type
                int64_t sBufSiz;

//                if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d\r\n", __FUNCTION__, __LINE__), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);
                Status1 = pCdeFile->pRootProtocol->SetPosition(pCdeFile->pFileProtocol, pCdeFile->gappos);      // set position
/*                if (trcen == 2)
                    swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d Status %hs, pCdeFile->gappos %lld, sGapSize %lld\r\n", 
                    __FUNCTION__, 
                    __LINE__, 
                    _strefierror(Status1), 
                    pCdeFile->gappos,
                        sGapSize),

                    _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);
*/
                do
                {

                    sGapSize -= sizeof(zBuf64k);                                                                // subtract buffer size
                    sBufSiz = sGapSize > 0 ? sizeof(zBuf64k) : sGapSize + sizeof(zBuf64k);                      // write full buffersize of zeros / write remaining zeros

//                    if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d, sGapSize %lld, sBufSiz %lld\r\n", __FUNCTION__, __LINE__, sGapSize, sBufSiz), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);

                    Status2 = pCdeFile->pRootProtocol->Write(pCdeFile->pFileProtocol, (UINTN*) &sBufSiz, zBuf64k);       // initialize file gap with zeros

//                    if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d, sGapSize %lld, sBufSiz %lld\r\n", __FUNCTION__, __LINE__, sGapSize, sBufSiz), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);

                } while (sGapSize > 0);                                                                         // untile nothing to to anymore
//                if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d\r\n", __FUNCTION__, __LINE__), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);
            }
        }
//        if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d\n", __FUNCTION__, __LINE__), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);
        BufferSize = nelem * elmsize;
        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Write(pCdeFile->pFileProtocol, &BufferSize, p));
        count = BufferSize / elmsize;

        *pUni = 'T' == *pUni ? 0 : *pUni;

    } while (0);
//    if (trcen == 2)swprintf(wcsbuf, INT_MAX, L"%hs(), Line %d\n", __FUNCTION__, __LINE__), _cdegST->ConOut->OutputString(_cdegST->ConOut, wcsbuf);

    return  EFI_SUCCESS == Status ? (size_t)count : 0;
}
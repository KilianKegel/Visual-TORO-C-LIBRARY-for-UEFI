/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellDirectoryCreate.c

Abstract:

    OS interface (osif) to create a directory

Author:

    Kilian Kegel

--*/
#define OS_EFI//parameter for CdeServices.h
#define _INC_STDDEF         // exclude MSFT STDDEF.H, that conflicts with errno
#include <CdeServices.h>
//
// stdlib.h
//
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

//
// string.h
// 
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);

//
// errno.h
//
#define errno   (*_errno())
#define ENOENT  2
#define EEXIST  17
extern __declspec(dllimport) int* _errno(void);

/*++
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellDirectoryCreate(IN CDE_APP_IF* pCdeAppIf, const char* strDirectoryName);
Description
    Create new directory, non-existance assumed
Paramters
    CDE_APP_IF* pCdeAppIf       : unused
    const char* strDirectoryName: directory name ASCII
Returns
    0: success
    errno : failure
*/
int _osifUefiShellDirectoryCreate(IN CDE_APP_IF* pCdeAppIf, wchar_t* pwcsDirName)
{
    int nRetErrno = -1, n;
                                                                        // for temporary usage only
    //printf(/*__FILE__*/"(%d), "__FUNCTION__"()"">>> pwcsDirName -> %ls\n", __LINE__, pwcsDirName);

    do {
        CDEFILE CdeFile, * pCdeFile = memset(&CdeFile, 0, sizeof(CDEFILE)); // file struct local, outside the _iob[] array

        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()"">>> pwcsDirName -> %ls, pCdeFile %p\n", __LINE__, pwcsDirName, pCdeFile);

        pCdeFile = pCdeAppIf->pCdeServices->pFopen(pCdeAppIf, pwcsDirName, "CdeMkDir", -1, pCdeFile);

        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""pCdeFile -> %p\n", __LINE__, pCdeFile);

        if (NULL == pCdeFile) {
            //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""break, errno %s\n", __LINE__, strerror(errno));
            nRetErrno = errno;
            break;
        }

        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""Fclose() -> pCdeFile %p\n", __LINE__, pCdeFile);

        n = pCdeAppIf->pCdeServices->pFclose(pCdeAppIf, pCdeFile);

        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""Fclose() returned -> %X\n", __LINE__, n);

        if (EXIT_SUCCESS != n) {
            /// <TODO>
            /// add errno
            /// </TODO>
            break;
        }

    } while (nRetErrno = EXIT_SUCCESS, nRetErrno == EXIT_SUCCESS);

    //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""nRet -> %X\n", __LINE__, nRetErrno);

    return nRetErrno;
}
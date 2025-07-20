/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Remove.c

Abstract:

    Implementation of the Standard C function.
    Delete a file.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <CdeServices.h>

/** remove
Synopsis
    #include <stdio.h>
    int remove(const char *filename);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/remove-wremove?view=msvc-160&viewFallbackFrom=vs-2019
    The remove function causes the file whose name is the string pointed to by filename
    to be no longer accessible by that name. A subsequent attempt to open that file using that
    name will fail, unless it is created anew. If the file is open, the behavior of the remove
    function is implementation-defined.
Returns
    The remove function returns zero if the operation succeeds, nonzero if it fails.
**/
int remove(const char* filename)
{

    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    CDEFILE* pCdeFile = NULL;
    int nRet = -1;

    do {

        // NOTE:EFI_FILE_PROTOCOL.Delete() has to be called on OPEN file!!!

        if (SHELLIF == pCdeAppIf->DriverParm.CommParm.OSIf)     // yes, is UEFI Shell interface ?
        {

            pCdeFile = (CDEFILE*)fopen(filename, "r+");

            if (NULL == pCdeFile)
                break;

        }

        nRet = pCdeAppIf->pCdeServices->pFdelete(pCdeAppIf, filename, pCdeFile);

        if (NULL != pCdeFile)
        {
            pCdeFile->fRsv = FALSE;     // clear reserved flasg
        }

    } while (0);

    return nRet;
}
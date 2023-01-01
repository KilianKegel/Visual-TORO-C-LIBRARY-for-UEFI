/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Fgetpos.c

Abstract:

    Implementation of the Standard C function.
    Gets a stream's file-position indicator.

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <stdio.h>
#include <CdeServices.h>

extern int __cdeIsFilePointer(void* stream);
/*
Synopsis
    #include <stdio.h>
    int fgetpos(FILE * restrict stream, fpos_t * restrict pos);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fgetpos?view=msvc-160&viewFallbackFrom=vs-2019
    The fgetpos function stores the current values of the parse state (if any) and file
    position indicator for the stream pointed to by stream in the object pointed to by pos.
    The values stored contain unspecified information usable by the fsetpos function for
    repositioning the stream to its position at the time of the call to the fgetpos function.
Returns
    If successful, the fgetpos function returns zero; on failure, the fgetpos function
    returns nonzero and stores an implementation-defined positive value in errno.
*/
int fgetpos(FILE* fp, fpos_t* pos)
{
    EFI_STATUS Status = EFI_DEVICE_ERROR;
    CDEFILE* pCdeFile = (CDEFILE*)fp;
    do {

        if (!__cdeIsFilePointer(pCdeFile))
            break;

        *pos = pCdeFile->bpos + pCdeFile->bidx;

        Status = EFI_SUCCESS;

    } while (0);

    //TODO: Add errno
    return EFI_SUCCESS == __cdeOnErrSet_status(Status) ? 0 : EOF;
}
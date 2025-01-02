/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
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

extern int __cdeIsFilePointer(void* stream);
extern int __cdeOnErrSet_errno(CDE_STATUS Status, int Error);
extern int __cdeIsCdeFposType(fpos_t fpos);
extern int __cdeBiasCdeFposType(fpos_t fpos);
extern fpos_t __cdeOffsetCdeFposType(fpos_t fpos);

/*
Synopsis
    #include <stdio.h>
    int fsetpos(FILE *stream, const fpos_t *pos);
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
int fsetpos(FILE* stream, const fpos_t* pos)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    return pCdeAppIf->pCdeServices->pFSetPosCORE(pCdeAppIf, (CDEFILE*)stream, pos);
}
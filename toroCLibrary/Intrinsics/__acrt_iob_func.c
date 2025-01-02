/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __acrt_iob_func.c

Abstract:

    Microsoft specific intrinsic function.
    The __acrt_iob_func() function retrieves the address of a given
    I/O buffer index.

Author:

    Kilian Kegel

--*/

#include <stdio.h>
#include <CdeServices.h>

/** __acrt_iob_func
Synopsis
    #include <stdio.h>
    FILE* __acrt_iob_func(  int _idxIOB  );
Description
    The __acrt_iob_func() function retrieves the address of a given
    I/O buffer index.
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/internal-crt-globals-and-functions?view=msvc-160
Returns
    on success: FILE*   pointer to the requested /O buffer.
    on error:   NULL,   if index is outside the allocated buffers 
                            or
                        CdeAppIf CDE Apllication Interface is not available, e.g.
                        this is not a CDE application. In this case additionally 

                        errno is set to EPERM
**/
FILE* __acrt_iob_func(  unsigned i  )
{
    CDEFILE* pRet = NULL;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    if (NULL != pCdeAppIf)
        if(i < (unsigned)pCdeAppIf->cIob)           // check that i is an range 0 .. < pCdeAppIf->cIob, but not 0xFFFFFFFF
            pRet = &pCdeAppIf->pIob[i];
    
    return (FILE*)pRet;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdsGetIOBufferCORE.c

Abstract:

    The __cdeGetIOBuffer() function retrieves the address of a given
    I/O buffer index.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void* __acrt_iob_funcCORE(CDE_APP_IF* pCdeAppIf, int i);
/**
Synopsis

    void* __cdeGetIOBufferCORE(CDE_APP_IF *pCdeAppIf,   unsigned _idxIOB  );

Description

    The __cdeGetIOBuffer() function retrieves the address of a given
    I/O buffer index.

Returns

    on success: FILE*   pointer to the requested /O buffer.
    on error:   NULL,   if index is outside the allocated buffers
                            or
                        CdeAppIf CDE Apllication Interface is not available, e.g.
                        this is not a CDE application. In this case additionally

                        errno is set to EPERM
**/
void* __cdeGetIOBufferCORE(CDE_APP_IF* pCdeAppIf, unsigned i)
{
    return __acrt_iob_funcCORE(pCdeAppIf, i);
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _set_invalid_parameter_handler.c

Abstract:

    Microsoft C Library specific replacement for CDE
    Dummy function for Microsoft _set_invalid_parameter_handler()
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler?view=msvc-160

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

#ifndef wchar_t
    #define  wchar_t short
#endif//wchar_t

extern void (*pinvalid_parameter_handlerCDEABI)(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,unsigned* pReserved);

static void  (*_set_invalid_parameter_handlerCDEABI(void (*pfn)(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,unsigned* )))(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,unsigned* ){

    void (*pRet)(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,unsigned* ) = pinvalid_parameter_handlerCDEABI;

    pinvalid_parameter_handlerCDEABI = pfn;
    
    return pRet;
}

MKCDEABI(_set_invalid_parameter_handler);
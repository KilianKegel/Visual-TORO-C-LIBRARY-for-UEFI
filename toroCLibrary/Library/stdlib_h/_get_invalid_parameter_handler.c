/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _get_invalid_parameter_handler.c

Abstract:

    Microsoft C Library specific function _get_invalid_parameter_handler()
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/get-invalid-parameter-handler-get-thread-local-invalid-parameter-handler

Author:

    Kilian Kegel

--*/
#include <stdint.h>
//
// stdlib.h
//
typedef void(__cdecl* _invalid_parameter_handler)(
    wchar_t const*,
    wchar_t const*,
    wchar_t const*,
    unsigned int,
    uintptr_t
    );

#ifndef wchar_t
    #define  wchar_t short
#endif//wchar_t

extern void (*_pcdeInvalidParameterHandler)(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,unsigned* pReserved);
extern void _cdeDefaultInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);

/**

Synopsis
    #include <stdlib.h>
    _invalid_parameter_handler* _get_invalid_parameter_handler(void);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/get-invalid-parameter-handler-get-thread-local-invalid-parameter-handler
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/get-invalid-parameter-handler-get-thread-local-invalid-parameter-handler#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/get-invalid-parameter-handler-get-thread-local-invalid-parameter-handler#return-value

**/
_invalid_parameter_handler* _get_invalid_parameter_handler(void)
{
    void *pRet = 0;//NULL

    if (_pcdeInvalidParameterHandler != _cdeDefaultInvalidParameterHandler)
        pRet = (void*)_pcdeInvalidParameterHandler;

    return pRet;
}

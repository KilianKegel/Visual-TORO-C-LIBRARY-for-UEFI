/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
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
//
// stdlib.h
// 
// Invalid parameter handler function pointer type
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

extern void (*_pcdeInvalidParameterHandler)(_invalid_parameter_handler);
extern void _cdeDefaultInvalidParameterHandler(_invalid_parameter_handler);

/**

Synopsis
    #include <stdlib.h>
    _invalid_parameter_handler* _set_invalid_parameter_handler(_invalid_parameter_handler fn);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler#return-value

**/
_invalid_parameter_handler* _set_invalid_parameter_handler(_invalid_parameter_handler fn){

    _invalid_parameter_handler* pRet = (void*)0;//NULL
    
    if (_pcdeInvalidParameterHandler != _cdeDefaultInvalidParameterHandler)
        pRet = (void*)_pcdeInvalidParameterHandler;

    _pcdeInvalidParameterHandler = (void (*)(_invalid_parameter_handler))fn;
    
    return pRet;
}
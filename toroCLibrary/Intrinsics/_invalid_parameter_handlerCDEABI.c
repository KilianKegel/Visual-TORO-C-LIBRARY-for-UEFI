/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _invalid_parameter_handlerCDEABI.c

Abstract:

    Import Library version

    Microsoft C Library specific replacement for CDE
    Dummy function for Microsoft _set_invalid_parameter_handler()
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler?view=msvc-160

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
// stdio.h
//
#define FILE void
#define stderr (__acrt_iob_func(2))
extern FILE* __acrt_iob_func(unsigned i);

extern __declspec(dllimport) int fprintf(FILE* stream, const char* pszFormat, ...);
extern __declspec(dllimport) void abort(void);

/**

Synopsis
    void invalid_parameter_handler(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,unsigned* pReserved);
Description
    https://msdn.microsoft.com/en-us/library/windows/desktop/a9yf33zb(v=vs.120).aspx
Returns

    @param[in] const wchar_t * expression
    @param[in] const wchar_t * function
    @param[in] const wchar_t * file
    @param[in] unsigned int line
    @param[in] unsigned* pReserved

    @retval void

**/
static void invalid_parameter_handler(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,unsigned * pReserved){

    if(NULL != file)
    {
        fprintf(stderr,"invalid parameter: %S(%d) %S() %S \n",file,line, function, expression);
        abort();
    }
    else
    {
        fprintf(stderr,"invalid parameter\n");
        abort();
    }
    return;

}
void (*pinvalid_parameter_handlerCDEABI)(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,unsigned* pReserved) = invalid_parameter_handler;

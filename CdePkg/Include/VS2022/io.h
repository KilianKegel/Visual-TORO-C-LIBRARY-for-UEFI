/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    io.h

Abstract:

    Partially implementation of the Microsoft C header file for the Microsoft C Compiler VS2022

Author:

    Kilian Kegel

--*/
#ifndef _CDE_IO_H_
#define _CDE_IO_H_
#include <stddef.h>

#ifdef _USE_32BIT_TIME_T
#   error _USE_32BIT_TIME_T not supported
#endif// _USE_32BIT_TIME_T

typedef unsigned long _fsize_t;

struct _finddata64i32_t
{
    unsigned    attrib;
    __time64_t  time_create;
    __time64_t  time_access;
    __time64_t  time_write;
    _fsize_t    size;
    char        name[260];
};

#define _findfirst      _findfirst64i32
#define _findnext       _findnext64i32

int _findclose(intptr_t _FindHandle);
int _findnext64i32(intptr_t _FindHandle,struct _finddata64i32_t* _FindData);
intptr_t _findfirst64i32(char const* _FileName,struct _finddata64i32_t* _FindData);

#define _A_NORMAL (0 << 0) /* Normal file - No read/write restrictions */
#define _A_RDONLY (1 << 0) /* Read only file                           */
#define _A_HIDDEN (1 << 1) /* Hidden file                              */
#define _A_SYSTEM (1 << 2) /* System file                              */
#define _A_SUBDIR (1 << 4) /* Subdirectory                             */
#define _A_ARCH   (1 << 5) /* Archive file                             */

#endif//_CDE_IO_H_

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    StrErrorCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Gets a system error message string

TODO: BUGBUG In the Microsoft implementation successive calls overwrite previeous error strings.

Author:

    Kilian Kegel

--*/
#include <assert.h>
#undef NULL
#include <Uefi\UefiBaseType.h>
#include <CdeServices.h>

static char* rgszSysErrors[] = {
    /*0:*/"No error",
    /*1:*/"Operation not permitted",
    /*2:*/"No such file or directory",
    /*3:*/"No such process",
    /*4:*/"Interrupted function call",
    /*5:*/"Input/output error",
    /*6:*/"No such device or address",
    /*7:*/"Arg list too long",
    /*8:*/"Exec format error",
    /*9:*/"Bad file descriptor",
    /*10:*/"No child processes",
    /*11:*/"Resource temporarily unavailable",
    /*12:*/"Not enough space",
    /*13:*/"Permission denied",
    /*14:*/"Bad address",
    /*15:*/"errnum = 15",
    /*16:*/"Resource device",
    /*17:*/"File exists",
    /*18:*/"Improper link",
    /*19:*/"No such device",
    /*20:*/"Not a directory",
    /*21:*/"Is a directory",
    /*22:*/"Invalid argument",
    /*23:*/"Too many open files in system",
    /*24:*/"Too many open files",
    /*25:*/"Inappropriate I/O control operation",
    /*26:*/"errnum = 26",
    /*27:*/"File too large",
    /*28:*/"No space left on device",
    /*29:*/"Invalid seek",
    /*30:*/"Read-only file system",
    /*31:*/"Too many links",
    /*32:*/"Broken pipe",
    /*33:*/"Domain error",
    /*34:*/"Result too large",
    /*35:*/"errnum = 35",
    /*36:*/"Resource deadlock avoided",
    /*37:*/"errnum = 37",
    /*38:*/"Filename too long",
    /*39:*/"No locks available",
    /*40:*/"Function not implemented",
    /*41:*/"Directory not empty",
    /*42:*/"Illegal byte sequence" };

/**
Synopsis
    #include <string.h>
    char* strerror(int errcode);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-strerror-wcserror-wcserror?view=msvc-160
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-strerror-wcserror-wcserror?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-strerror-wcserror-wcserror?view=msvc-160#return-value
**/
static char* strerrorCDEABI(int errcode) {

    char* pszRet = "Unknown error";

    if ((unsigned int)errcode < sizeof(rgszSysErrors) / sizeof(char*)) {
        pszRet = rgszSysErrors[errcode];
    }

    return pszRet;
}

MKCDEABI(strerror);
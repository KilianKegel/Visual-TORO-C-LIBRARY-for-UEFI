/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _WcsEfiError.c

Abstract:

    Import Library version

    Toro C Library specific function for UEFI subsystems
    Gets a system error message string

Author:

    Kilian Kegel

--*/
#include <Uefi\UefiBaseType.h>
#include <CdeServices.h>
#include <stddef.h>
static wchar_t* rgszEfiErrors[] = {/*EdkCompatibilityPkg\Foundation\Efi\Include\EfiError.h*/
    L"Success",               /* 0           */
    L"Load error",            /* efierr (1)  */
    L"Invalid parameter",     /* efierr (2)  */
    L"Unsupported",           /* efierr (3)  */
    L"Bad buffer size",       /* efierr (4)  */
    L"Buffer too small",      /* efierr (5)  */
    L"Not ready",             /* efierr (6)  */
    L"Device error",          /* efierr (7)  */
    L"Write protected",       /* efierr (8)  */
    L"Out of resources",      /* efierr (9)  */
    L"Volume corrupted",      /* efierr (10) */
    L"Volume full",           /* efierr (11) */
    L"No media",              /* efierr (12) */
    L"Media changed",         /* efierr (13) */
    L"Not found",             /* efierr (14) */
    L"Access denied",         /* efierr (15) */
    L"No response",           /* efierr (16) */
    L"No mapping",            /* efierr (17) */
    L"Timeout",               /* efierr (18) */
    L"Not started",           /* efierr (19) */
    L"Already started",       /* efierr (20) */
    L"Aborted",               /* efierr (21) */
    L"Icmp error",            /* efierr (22) */
    L"Tftp error",            /* efierr (23) */
    L"Protocol error",        /* efierr (24) */
    L"Incompatible version",  /* efierr (25) */
    L"Security violation",    /* efierr (26) */
    L"Crc error",             /* efierr (27) */
    L"End of media",          /* efierr (28) */
    L"End of file",           /* efierr (31) */
    L"Invalid language",      /* efierr (32) */
};
/**

Synopsis
    wchar_t* _wcsefierror(EFI_STATUS errcode);
Description
    _wcsefierror() is UEFI equivavlent of Standard C strerror()
Parameters
    EFI_STATUS errcode
Returns
    Pointer to corresponding error string
    
**/
static wchar_t* _wcsefierrorCDEABI(EFI_STATUS errcode) {

    static wchar_t* pszRet = { L"Unknown error" };
    unsigned i;

    EFI_STATUS ErrMask = (1ULL << (8 * sizeof(EFI_STATUS) - 1));

    do {

        if ((0 == (errcode & ErrMask)) && (0 != errcode))
            break;

        errcode &= ~ErrMask;
        i = (unsigned)errcode;

        if (i < (sizeof(rgszEfiErrors) / sizeof(char*)) )
            pszRet = rgszEfiErrors[i];

    } while (0);

    return pszRet;
}

MKCDEABI(_wcsefierror);
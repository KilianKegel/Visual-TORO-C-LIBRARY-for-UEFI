/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _StrEfiError.c

Abstract:

    Toro C Library specific function for UEFI subsystems
    Gets a system error message string

Author:

    Kilian Kegel

--*/
#include <assert.h>
#undef NULL
#include <Uefi\UefiBaseType.h>
static char* rgszEfiErrors[] = {/*EdkCompatibilityPkg\Foundation\Efi\Include\EfiError.h*/
    "Success",               /* 0           */
    "Load error",            /* efierr (1)  */
    "Invalid parameter",     /* efierr (2)  */
    "Unsupported",           /* efierr (3)  */
    "Bad buffer size",       /* efierr (4)  */
    "Buffer too small",      /* efierr (5)  */
    "Not ready",             /* efierr (6)  */
    "Device error",          /* efierr (7)  */
    "Write protected",       /* efierr (8)  */
    "Out of resources",      /* efierr (9)  */
    "Volume corrupted",      /* efierr (10) */
    "Volume full",           /* efierr (11) */
    "No media",              /* efierr (12) */
    "Media changed",         /* efierr (13) */
    "Not found",             /* efierr (14) */
    "Access denied",         /* efierr (15) */
    "No response",           /* efierr (16) */
    "No mapping",            /* efierr (17) */
    "Timeout",               /* efierr (18) */
    "Not started",           /* efierr (19) */
    "Already started",       /* efierr (20) */
    "Aborted",               /* efierr (21) */
    "Icmp error",            /* efierr (22) */
    "Tftp error",            /* efierr (23) */
    "Protocol error",        /* efierr (24) */
    "Incompatible version",  /* efierr (25) */
    "Security violation",    /* efierr (26) */
    "Crc error",             /* efierr (27) */
    "End of media",          /* efierr (28) */
    "End of file",           /* efierr (31) */
    "Invalid language",      /* efierr (32) */
};
/**

Synopsis
    char* _strefierror(EFI_STATUS errcode);
Description
    _strefierror() is UEFI equivavlent of Standard C strerror()
Parameters
    EFI_STATUS errcode
Returns
    Pointer to corresponding error string
    
**/
char* _strefierror(EFI_STATUS errcode) {

    char* pszRet = { "Unknown error" };
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
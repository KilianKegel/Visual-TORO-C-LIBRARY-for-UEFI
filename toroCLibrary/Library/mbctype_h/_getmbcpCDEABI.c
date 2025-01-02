/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _getmbcp.c

Abstract:

    Import Library version

    Microsoft C Library specific function _getmbcp()
    Retrieves the current code page.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/**

Synopsis
    #include <mbctype.h>
    int _getmbcp( void );
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getmbcp
    For TORO-C-Library _getmbcp() return always 0, because multi byte support is not implemented.
    NOTE:   Multibyte codepages are not supported by TORO C LIBRARY.

    @param[in]

    @retval 0 on success


**/
static int _getmbcpCDEABI(void) 
{
    return 0;
}

MKCDEABI(_getmbcp);
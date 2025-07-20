/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _setmbcp.c

Abstract:

    Microsoft C Library specific function _setmbcp()
    Sets a new multibyte code page.

Author:

    Kilian Kegel

--*/
#include <errno.h>

//
// single byte codepages known to Microsoft C VS2022
//
extern unsigned short _cdeMsftSingleByteCodePageNumber[];

/**

Synopsis
    #include <mbctype.h>
    int _setmbcp(int codepage);
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmbcp
    The _setmbcp() specifies a new multibyte code page.
    NOTE:   Multibyte codepages are not supported by TORO C LIBRARY.
            Any attempt to set codepage to a multibyte code page will fail.
            All single byte code pages are known to the library.

    @param[in] codepage

    @retval 0 on success

    @retval -1 otherwise

**/
int _setmbcp(int codepage) {

    int i, nRet = 0;

    do {

        for (i = 0; (unsigned short) -1 != _cdeMsftSingleByteCodePageNumber[i]; i++)
            if ((unsigned short)codepage == _cdeMsftSingleByteCodePageNumber[i])
                break;//for

        if ((unsigned short)-1 != _cdeMsftSingleByteCodePageNumber[i])
            break;

        //
        // set error
        //
        errno = EINVAL;
        
        nRet = -1;

    } while (0);
    
    return nRet;
}


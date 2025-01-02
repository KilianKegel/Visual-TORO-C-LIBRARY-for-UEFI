/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeFileonly.c

Abstract:

    CDE internal removes path from filename

Author:

    Kilian Kegel

--*/

/** _CdeFileonly()

Synopsis

    char * _CdeFileonly(char *pszFile)

Description

    CDE internal removes path from filename

Returns

    @param[in] char *pszFile

    @retval pszFileWOPath

**/

char * _CdeFileonly(char *pszFile)
{
    int i = 0;

        while('\0' != pszFile[i])
            i++;

        while( (0 != i) && ('\\' != pszFile[i]) )
            i--;

        return &pszFile[i];
}

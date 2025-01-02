/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    iswalnumCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
#define _CTYPE_DISABLE_MACROS
#include <CdeServices.h>

extern __declspec(dllimport) int iswalpha(wint_t c);
extern __declspec(dllimport) int iswdigit(wint_t c);

/** Brief description of the function’s purpose.

Synopsis
    #include <wctype.h>
    int iswalnum(wint_t wc);
Description
    The iswalnum function tests for any wide character for which iswalpha or
    iswdigit is true.

    @param[in] c character to test for alnum

    @retval eighter ISLOWER or ISUPPER or
     ISDIGIT if alnum letter

    @retval 0 if not

**/
static int iswalnumCDEABI(wint_t c) {
    int nRet0 = 0, nRet1 = 0;

    nRet0 = iswalpha(c);

    if (0 == nRet0)
        nRet1 = iswdigit(c);

    return nRet0 | nRet1;
}

MKCDEABI(iswalnum);
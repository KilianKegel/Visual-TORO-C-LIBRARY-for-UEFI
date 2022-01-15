/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreExit.c

Abstract:

    exit() for core mode (that is: CDE_APP_IF* pCdeAppIf is parameter)

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stdarg.h>
#include <limits.h>

void _cdeCoreExit(CDE_APP_IF* pCdeAppIf, int status) {

    pCdeAppIf->exit_status = status;
    longjmp(pCdeAppIf->exit_buf, status);
}
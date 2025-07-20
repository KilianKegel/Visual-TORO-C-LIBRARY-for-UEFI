/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _getdcwd.c

Abstract:

    Implementation of the Standard C function

Author:

    Kilian Kegel

--*/
#include <stdio.h>
//#include <CdeServices.h>
//
//
///** _getdcwd
//Synopsis
//    #include <direct.h>
//    char* _getdcwd(int drive, char* buffer, int maxlen);
//Description
//    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getdcwd-wgetdcwd?view=msvc-160
//Returns
//    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getdcwd-wgetdcwd?view=msvc-160#return-value
//**/
//char* _getdcwd(int drive, char* buffer, int maxlen)
//{
//
//// bei drive n/a: 13 Permission denied       EACCES
//// bei BUFFER TOO SMALL: 34 Result too large ERANGE
//// BEI ??? 12 ENOMEM
//
//    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
//    char* pRet = NULL;
//
//    do
//    {
//        if (SHELLIF == pCdeAppIf->DriverParm.CommParm.OSIf)
//        {
//
//        }
//
//    } while (0);
//
//
//    return pRet;
//
//}
#include <CdeServices.h>
static char filler[] = { " warning C4206: nonstandard extension used: translation unit is empty" };
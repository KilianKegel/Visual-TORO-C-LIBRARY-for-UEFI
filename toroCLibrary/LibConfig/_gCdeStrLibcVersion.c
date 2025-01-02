/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _gCdeStrLibcVersion.c

Abstract:

    CDE internal:
    Toro C Library version string, like "Toro C Library v1.2.3 Build 4"

Author:

    Kilian Kegel

--*/
#include <VERSION.h>
#include <BUILDNUM.h>
#include <CdeServices.h>

#define VERSIONSTRING "Toro C Library Version "CDENUMTOSTR(MAJORVER)"."CDENUMTOSTR(MINORVER)"."CDENUMTOSTR(PATCHVER)" Build "CDENUMTOSTR(BUILDNUM)

char _gCdeStrLibcVersion[] = VERSIONSTRING;

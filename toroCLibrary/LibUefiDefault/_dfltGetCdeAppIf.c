/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _dfltGetCdeAppIf.c

Abstract:

    Default / empty Get-Application-Interface-function to satisfy the linker
    requirements.

    NOTE: It returns NULL, so the Cde Application Interface is not available
    for driver -- it is a non-CdePkg-driver.

Author:

    Kilian Kegel

--*/
#include <cde.h>
#include <CdeServices.h>

void* __cdeGetAppIf(void) 
{
    return NULL;
}

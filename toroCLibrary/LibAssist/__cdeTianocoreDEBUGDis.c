/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeTianocoreDEBUGDis.c

Abstract:

    Disable Torito C Library / CdePkg 's DebugVPrint() reimplementation at runtime.

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>

/**

Synopsis
    void __cdeTianocoreDEBUGDis(void);
Description
    Disable Torito C Library / CdePkg 's DebugVPrint() reimplementation at runtime.
    This is required in a RELEASE BIOS when BaseDebugLibNull.lib is normally 
    pulled in but for CdePkg based drivers DebugVPrint() is already bound to
    the entry point.
    In this case a RELEASE BIOS would send trace messages.
Returns

    @param[in]

    @retval 

**/
void __cdeTianocoreDEBUGDis(void)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    if (NULL != pCdeAppIf)
        pCdeAppIf->bmRuntimeFlags &= ~TIANOCOREDEBUG;

}


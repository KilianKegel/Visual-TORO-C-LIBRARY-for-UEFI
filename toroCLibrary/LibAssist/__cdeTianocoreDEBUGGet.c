/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeTianocoreDEBUGGet.c

Abstract:

    Get Torito C Library / CdePkg 's DebugVPrint() reimplementation enable/disable state at runtime.

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>

/**

Synopsis
    char __cdeTianocoreDEBUGGet(CDE_APP_IF* pCdeAppIf);
Description
    Get Torito C Library / CdePkg 's DebugVPrint() reimplementation enable/disable state at runtime.
    This is required in a RELEASE BIOS when BaseDebugLibNull.lib is normally
    pulled in but for CdePkg based drivers DebugVPrint() is already bound to
    the entry point.
    In this case a RELEASE BIOS would send trace messages.
Returns

    @param[in] CDE_APP_IF* pCdeAppIf

    @retval 1 : DebugVPrint() is enabled
            0 : DebugVPrint() is disabled

**/
char __cdeTianocoreDEBUGGet(CDE_APP_IF* pCdeAppIf)
{
    char fRet = 0;
    
    if (NULL != pCdeAppIf)
        fRet =  0 != (TIANOCOREDEBUG & pCdeAppIf->bmRuntimeFlags);

    return fRet;

}


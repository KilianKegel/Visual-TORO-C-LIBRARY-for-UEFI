/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _invparm.h

Abstract:

    Dummy function for Microsoft _set_invalid_parameter_handler()
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler?view=msvc-160

Author:

    Kilian Kegel

--*/
#ifndef _INVPARM_H_
#define _INVPARM_H_
#if _MSC_VER > 1500
#include <stdlib.h>
#include <crtdbg.h>                     // _CrtSetReportMode
void invparm(
    const wchar_t* expression,
    const wchar_t* function,
    const wchar_t* file,
    unsigned int line,
    unsigned* pReserved
) {
    return;
}
#endif// __MSC_VER > 1500

#endif//_INVPARM_H_

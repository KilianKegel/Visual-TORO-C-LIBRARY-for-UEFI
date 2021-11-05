/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeOnErrSet_status.c

Abstract:

    Implementation of the Standard C function.

Author:

    Kilian Kegel

--*/
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage __cdeOnErrSet_status

\brief Implementation of the <em>__cdeOnErrSet_status</em> function

\file __cdeOnErrSet_status.c

*/
#include <CdeServices.h>
#include <errno.h>

//extern CDE_STATUS efierrno;    // last CDE_STATUS before returning to calling function
extern CDE_STATUS/*EFI_STATUS*/* _efierrno(void);

CDE_STATUS __cdeOnErrSet_status(CDE_STATUS Status) {    //NOTE: This function is published in CdeServices.h, since it is my own invention and not defined in ERRNO.H per specification

    if (CDE_SUCCESS != Status)
        *_efierrno() = Status;

    return Status;

}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeAbort.c

Abstract:

    Toro C Library specific signal default handler.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**

Synopsis
    void _cdeAbort(void);
Description
    Toro C Library internal abort function invoked by the default signal handler.
    It _doesn't_ write "abnormal program termination" to stderr
    and calls the exit(3) function.
Returns
    does not return
**/
void _cdeAbort(void) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int i;

    // ----- don't flush files

    if (pCdeAppIf->pIob != (CDEFILE*)-1)
        for (i = 3/*skip stdin,stdout,stderr*/; i < pCdeAppIf->cIob; i++)
            pCdeAppIf->pIob[i].bdirty = 0;

    exit(3); //NOTE: Return 3 as documented by Microsoft
}

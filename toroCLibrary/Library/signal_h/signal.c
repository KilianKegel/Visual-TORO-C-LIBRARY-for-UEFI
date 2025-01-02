/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    signal.c

Abstract:

    Implementation of the Standard C function.
    Sets interrupt signal handling.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <signal.h>

extern char _cdeSig2idx(int sig);
extern void _cdeSigDflt(int sig);
extern void _cdeSigIgn(int sig);

/**

Synopsis
    #include <signal.h>
    void (*signal(int sig, void (*func)(int)))(int);
Description

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/signal?view=msvc-160&viewFallbackFrom=vs-2019

Parameters

    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/signal?view=msvc-160#parameters

Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/signal?view=msvc-160#return-value

    @param[in] sig signal number
    @param[in] func address of handler

    @retval eighter ISLOWER or ISUPPER or
    @retval 0 if not

**/
void (*signal(int sig, void (*func)(int)))(int)
{
    void (*pRet)(int) = (void (*)(int)) - 1;
    char idx = _cdeSig2idx(sig);
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    do {

        if (idx < 0)
            break;                      //return -1

            //NOTE: raise() does not reinstall the default handler, as expected, but some garbage address
            //      that is returned by signal(), once the signal was raised...
            //MSFT BUGBUG pCdeAppIf->rgfnSignal[idx] = &_cdeSigDflt;
        if (pCdeAppIf->rgfnSignal[idx] == &_cdeSigDflt) {
            pRet = SIG_DFL;
        }
        else if (pCdeAppIf->rgfnSignal[idx] == &_cdeSigIgn) {
            pRet = SIG_IGN;
        }
        else {
            pRet = pCdeAppIf->rgfnSignal[idx];
        }

        if (func == SIG_DFL) {
            pCdeAppIf->rgfnSignal[idx] = &_cdeSigDflt;
        }
        else if (func == SIG_IGN) {
            pCdeAppIf->rgfnSignal[idx] = &_cdeSigIgn;
        }
        else {
            pCdeAppIf->rgfnSignal[idx] = func;
        }

    } while (0);

    return (pRet);
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    raiseCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Sends a signal to the executing program.

Author:

    Kilian Kegel

--*/

#include <CdeServices.h>
//
// signal.h
//
#define SIGILL  4
#define SIGSEGV 11

extern char _cdeSig2idx(int sig);
extern void (*pinvalid_parameter_handlerCDEABI)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);
extern void _cdeSigDflt(int sig);
extern void _cdeSigIgn(int sig);


/**

Synopsis
    #include <signal.h>
    int raise(int sig);
Description
    The raise function carries out the actions described in 7.14.1.1 for the signal sig. If a
    signal handler is called, the raise function shall not return until after the signal handler
    does.
Returns
    The raise function returns zero if successful, nonzero if unsuccessful.

    @param[in] sig signal number

    @retval 0 or doesn't return

**/
static int raiseCDEABI(int sig) {

    void (*sighnd)(int);
    char idx = _cdeSig2idx(sig);
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

    do {
        if (idx < 0){
            //(*pinvalid_parameter_handlerCDEABI)(L"\"idx < 0\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
            (*pinvalid_parameter_handlerCDEABI)(NULL, NULL, NULL, 0, 0);
        }
        else {
            sighnd = pCdeAppIf->rgfnSignal[idx];

            if (pCdeAppIf->rgfnSignal[idx] != &_cdeSigIgn) {
                //NOTE: raise() does not reinstall the default handler for signal number SIGILL and SIGSEGV,
                //      as expected, but some garbage address
                //      that is returned by signal(), once the signal was raised...
                //MSFT BUGBUG pCdeAppIf->rgfnSignal[idx] = &_cdeSigDflt;

                if (sig == SIGILL || sig == SIGSEGV)
                    pCdeAppIf->rgfnSignal[idx] = (void(__cdecl*)(int))0x6AABA6E;//&_cdeSigDflt; GARBAGE
                else
                    pCdeAppIf->rgfnSignal[idx] = &_cdeSigDflt;//&_cdeSigDflt;
            }

            (*sighnd)(sig);
        }
    } while (0);

    return(0);
}

MKCDEABI(raise);
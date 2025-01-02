/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    abortCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Sends a signal to the executing program.

Author:

    Kilian Kegel

--*/
//typedef union _XDUMPPARM {
//    unsigned reg;
//    struct {
//        unsigned ElmSizeMin1 : 3;       /*!<element size minus one, only 0,1,3,7                    */
//        unsigned AddrSize : 3;          /*!<0,1,2,3,4 with:
//                                            0 == "%016llX: "
//                                            1 == "%08llX: "
//                                            2 == "%04llX: "
//                                            3 == "%02llX: "
//                                            4 == ""                                                 */
//        unsigned ElmsPerLine : 7;       /*!< nBytesPerLine minus one bytes per line - 0 == 16,   */
//        unsigned NoAscii : 1;           /*!<append NO ASCII characters                              */
//        unsigned BaseOfs : 1;           /*!<base and offset, offset only otherwise                  */
//        unsigned NoDash : 1;            /*!<print dash "-" in between                               */
//        unsigned Pitch : 8;             /*!<pitch between two consecutive elements fo size elmsize  */
//    }bit;
//}XDUMPPARM; 
#define _CDE_H_
#include <CdeServices.h>

#define FILE void
extern FILE* __acrt_iob_func(unsigned i);
#define stderr (__acrt_iob_func(2))
extern __declspec(dllimport) int fprintf(FILE* stream, const char* pszFormat, ...);
extern __declspec(dllimport) int raise(int sig);
extern __declspec(dllimport) void exit(int status);

/** Brief description of the function’s purpose.

Synopsis
    #include <stdlib.h>
    void abort(void);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/abort?view=msvc-160&viewFallbackFrom=vs-2019
    The abort function causes abnormal program termination to occur, unless the signal
    SIGABRT is being caught and the signal handler does not return. Whether open streams
    with unwritten buffered data are flushed, open streams are closed, or temporary files are
    removed is implementation-defined. An implementation-defined form of the status
    unsuccessful termination is returned to the host environment by means of the function
    call raise(SIGABRT).

    NOTE/MSDN:

    If the Windows error reporting handler is not invoked, then abort calls _exit to terminate
    the process with exit code 3 and returns control to the parent process or the operating system.
    _exit does not flush stream buffers or do atexit/_onexit processing.

Returns
    The abort function does not return to its caller.

    @retval 3 to the shell

**/
static void abortCDEABI(void) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int i;

    fprintf(stderr, "abnormal program termination\n"); // MSFT: this message appears only in VS 6 / _MSC_VER == 1200
    raise(22/*SIGABRT*/);

    // ----- don't flush files

    for (i = 3/*skip stdin,stdout,stderr*/; i < pCdeAppIf->cIob; i++)
    {
        CDEFILE* fp = (CDEFILE*)__cdeGetIOBuffer(i);

        if (NULL == fp)
            break;

        fp->bdirty = 0;
        //_iob[i].bdirty = 0;
    }

    exit(0xC0000409/*STATUS_STACK_BUFFER_OVERRUN*/); //NOTE: Returnvalue of 3 documented by Microsoft instead
}

MKCDEABI(abort);
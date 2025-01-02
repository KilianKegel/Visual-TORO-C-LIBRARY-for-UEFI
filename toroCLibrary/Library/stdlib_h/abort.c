/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    abort.c

Abstract:

    Implementation of the Standard C function.
    Sends a signal to the executing program.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

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
void abort(void) {

    fprintf(stderr, "abnormal program termination\n"); // MSFT: this message appears only in VS 6 / _MSC_VER == 1200
    raise(SIGABRT);

    _exit(0xC0000409/*STATUS_STACK_BUFFER_OVERRUN*/); //NOTE: Returnvalue of 3 documented by Microsoft instead
}

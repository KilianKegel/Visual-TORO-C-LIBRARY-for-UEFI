/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    errno.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=198

Author:

    Kilian Kegel

--*/
#ifndef _CDE_ERRNO_H_
#define _CDE_ERRNO_H_

#define errno (*_errno())

#define EDOM	0x21
#define ERANGE  0x22
#define EILSEQ  0x2A	/*C99 defined*/

// Microsoft specific Error codes
//https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-constants?view=msvc-160
#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7   //Argument list too long
#define ENOEXEC         8
#define EBADF           9   //Bad file number.There are two possible causes : 1) The specified file descriptor is not a valid value or does not refer to an open file. 2) An attempt was made to write to a file or device opened for read - only access.
#define ECHILD          10  //No spawned processes
#define EAGAIN          11  //No more processes.An attempt to create a new process failed because there are no more process slots, or there is not enough memory, or the maximum nesting level has been reached.
#define ENOMEM          12  //Not enough core.Not enough memory is available for the attempted operator. For example, this message can occur when insufficient memory is available to execute a child process, or when the allocation request in a _getcwd call cannot be satisfied.
#define EACCES          13  //Permission denied.The file's permission setting does not allow the specified access. This error signifies that an attempt was made to access a file (or, in some cases, a directory) in a way that is incompatible with the file's attributes.
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
//#define EDOM            33
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41
// Error codes used in the Secure CRT functions
#define _SECURECRT_ERRCODE_VALUES_DEFINED
#define EINVAL          22
//#define ERANGE          34
//#define EILSEQ          42
#define STRUNCATE       80

int* _errno( void );

#endif//_CDE_ERRNO_H_

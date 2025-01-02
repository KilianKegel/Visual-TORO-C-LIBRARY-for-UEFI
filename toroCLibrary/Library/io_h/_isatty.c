/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _isatty.c

Abstract:

    Implementation of the POSIX C function.
    Determines whether a file descriptor is associated with a character device.

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <errno.h>
#include <CdeServices.h>

extern int __cdeIsFilePointer(void* stream);
extern void (*pinvalid_parameter_handler)(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, unsigned* pReserved);

/** isatty()

Synopsis
    #include <io.h>
    int _isatty( int fd );
Description
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/isatty?view=msvc-170
    https://pubs.opengroup.org/onlinepubs/9699919799/functions/isatty.html
Parameters
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/isatty?view=msvc-170#parameters
Returns
    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/isatty?view=msvc-170#return-value

**/
int _isatty(int fd)
{
    CDEFILE* pCdeFile = __cdeGetIOBuffer(fd);
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int nRet = 0;
    int fIsInPOST = FALSE;


    if (NULL == pCdeAppIf)
        errno = EPERM;
    else do 
    {
        //
        // check POST. During POST FILE/pCdeFile is not supported and always returns "associated with a character device"
        //
        switch (pCdeAppIf->DriverParm.CommParm.OSIf)
        {
            case PEIIF:
            case DXEIF:
            case SMMIF:
                fIsInPOST = TRUE;
                break;
            default:
                fIsInPOST = FALSE;
                break;
        }

        if (0 <= fd && fd <= 2)
        {
            if (TRUE == fIsInPOST)
            {
                nRet = 64;  // "associated with a character device"
                break;//while(0);
            }
        }

        if (NULL == pCdeFile || 0 == pCdeFile->fRsv || -1 == fd)
        {
            //(*pinvalid_parameter_handler)(L"\"NULL == pCdeFile || 0 == pCdeFile->fRsv || -1 == fd\"", __CDEWCSFUNCTION__, __CDEWCSFILE__, __LINE__, 0);
            (*pinvalid_parameter_handler)(NULL, NULL, NULL, 0, 0);
            errno = EBADF;
            break;//while(0);
        }

        if(0 <= fd && fd <= 2)
            if(0 == (pCdeFile->openmode & (O_CDEREDIR + O_CDEREOPEN)))
                nRet = 64;  // "associated with a character device"

    } while (0);

    return nRet;
}
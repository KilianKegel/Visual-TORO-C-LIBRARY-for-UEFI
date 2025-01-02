/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    getenv.c

Abstract:

    Implementation of the Standard C function.
    Gets a value from the current environment.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

/*++

Synopsis
    #include <stdlib.h>
    char *getenv(const char *name);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getenv-wgetenv?view=msvc-160&viewFallbackFrom=vs-2019
    The getenv function searches an environment list, provided by the host environment,
    for a string that matches the string pointed to by name. The set of environment names
    and the method for altering the environment list are implementation-defined.
    The implementation shall behave as if no library function calls the getenv function.
Returns
    The getenv function returns a pointer to a string associated with the matched list
    member. The string pointed to shall not be modified by the program, but may be
    overwritten by a subsequent call to the getenv function. If the specified name cannot
    be found, a null pointer is returned.

    @param[in] szEnvarCmd
    @retval on SUCCESS: pointer to environment variable
    @retval on FAILURE: NULL

*/
char* getenv(const char* pszEnvarCmd) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char* pRet = NULL;

    if (NULL != pszEnvarCmd) {
        pRet = pCdeAppIf->pCdeServices->pGetEnv(pCdeAppIf, pszEnvarCmd);
    }

    return pRet;
}

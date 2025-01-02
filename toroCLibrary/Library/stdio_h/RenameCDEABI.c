/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    RenameCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Rename a file or directory.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>

extern void* __cdeGetAppIf(void);
extern __declspec(dllimport) int _strnicmp(const char* pszDst, const char* pszSrc, size_t count);
extern __declspec(dllimport) int swprintf(wchar_t* pszDest, size_t dwCount, const wchar_t* pszFormat, ...);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strrchr(const char* str, int c);
extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);

static char* chkpath(const char* pOld, const char* pNew); // prototype

/*++
Synopsis
    #include <stdio.h>
    int rename(const char *old, const char *new);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rename-wrename?view=msvc-160&viewFallbackFrom=vs-2019
    The rename function causes the file whose name is the string pointed to by old to be
    henceforth known by the name given by the string pointed to by new. The file named
    old is no longer accessible by that name. If a file named by the string pointed to by new
    exists prior to the call to the rename function, the behavior is implementation-defined.
 Returns
    The rename function returns zero if the operation succeeds, nonzero if it fails in
    which case if the file existed previously it is still known by its original name.

*/
static int renameCDEABI(const char* pszOld, const char* pszNew) {
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    int nRet = 0;

    do {

        if (NULL == pszOld || NULL == pszNew)
            break;

        //CDEMOFINE((MFNINF(1) "Old: %s, New: %s\n", pszOld, pszNew));


        if (NULL != chkpath(pszOld, pszNew)) { // check, if criterias are matched, e.g.: renaming inside the same directory

            size_t oldsize = sizeof(wchar_t) * (1 + strlen(pszOld));
            size_t newsize = sizeof(wchar_t) * (1 + strlen(pszNew));
            wchar_t* pwcsOld = malloc(oldsize);
            wchar_t* pwcsNew = malloc(newsize);


            swprintf(pwcsOld, oldsize, L"%S", pszOld);
            swprintf(pwcsNew, newsize, L"%S", pszNew);

            //CDEMOFINE((MFNINF(1) "pwcsOld: %S, pwcsNew: %S\n", pwcsOld, pwcsNew));

            nRet = pCdeAppIf->pCdeServices->pFrename(pCdeAppIf, pwcsOld, pwcsNew);

            {
                nRet = nRet == 1 ? 0 : -1;  // 0 == noerror, -1 == error
            }
            free(pwcsOld);
            free(pwcsNew);
        }
        //CDEMOFINE((MFNINF(1) "%d\n", nRet));
    } while (0);
    return nRet;

}

static char* chkpath(const char* pOld, const char* pNew) {
    /*++

    Routine Description:

        This function checks, if the both given pathnames + filenames matches the criterias:

        1. if pathnames are given, they must be equal
        2. that means, renaming is only allowed in the same directory
        3. the Windows MoveFile() is used but inter-directory moves/renames are blocked in this implementation

    Arguments:

        @param[in] const char *pOld
        @param[in] const char *pNew

    Return Value:

        @retval on SUCCESS: pointer to new file name w/o path
        @retval on FAILURE: NULL

    */

    size_t size;
    char* pStrold, * pStrnew, * pRet = NULL;
    int equ;

    //
    // check for a given path, if path, then equal path on both sides!!!  Caseinsensitive
    //
    do {
        pStrold = strrchr(pOld, '\\');  // search for backslash
        pStrnew = strrchr(pNew, '\\');  // search for backslash

        if (NULL == pStrold) {          // if no path for the old file...
            if (NULL != pStrnew)        // ... there can not be a path for the new file
                break;                  // break and return NULL if this in not true
            pRet = ++pStrnew;
        }
        else {                          // path for old available and required for the new one too...
            if (NULL == pStrnew) {      // path missing, but required
                break;
            }
            else {
                size = 1 + pStrold - pOld;
                equ = !_strnicmp(pOld, pNew, size);

                if (equ)
                    pRet = ++pStrnew;
            }
        }
    } while (0);

    return pRet;
}

MKCDEABI(rename);
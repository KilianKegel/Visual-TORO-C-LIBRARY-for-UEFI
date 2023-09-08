//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileRename

\brief Implementation of the <em>osifLinuxFileRename</em> function

\file osifLinuxFileRename.c

*/
#include <cde.h>
#include <CdeServices.h>
#include <errno.h>
//
// stdlib.h
// 
extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) size_t wcstombs(char* mbstr, const wchar_t* wcstr, size_t count);
//
// wchar.h
//
extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);

//
// prototype
//
OSIFFRENAME _osifLinuxFileRename;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

int32_t _osifLinuxFileRename(IN CDE_APP_IF* pCdeAppIf, wchar_t* pwcsOld, wchar_t* pwcsNew)
{
    int64_t nRet = -1LL;
    long long fd64;
    char* pstrOld = malloc(sizeof("") + wcslen(pwcsOld));
    char* pstrNew = malloc(sizeof("") + wcslen(pwcsNew));

    do
    {
        wcstombs(pstrNew, pwcsNew, (size_t)-1);
        wcstombs(pstrOld, pwcsOld, (size_t)-1);
        //
        // test, if source file already exists -- since sys_rename just overwrites it if not write protected
        //
        fd64 = _syscall(2, (int64_t)pstrOld, /*LINUX_O_RDONLY*/ 0, /*S_IRWXU*/  00700, 0, 0, 0);
        if (fd64 < 0)
        {
            errno = ENOENT; //errno -> 2 == No such file or directory
            break;//while(0)
        }
        _syscall(/* sys_close */ 3, fd64, 0, 0, 0, 0, 0);
        
        //
        // test, if destination file already exists -- since sys_rename just overwrites it if not write protected
        //
        fd64 = _syscall(2, (int64_t)pstrNew, /*LINUX_O_RDONLY*/ 0, /*S_IRWXU*/  00700, 0, 0, 0);

        if (fd64 >= 0)
        {
            _syscall(/* sys_close */ 3, fd64, 0, 0, 0, 0, 0);
            errno = EEXIST; //EEXIST; //errno -> 17 == File exists
            break;//while(0)
        }

        nRet = _syscall(/* sys_rename */ 82, (int64_t)pstrOld, (int64_t)pstrNew, 0, 0, 0, 0);
        
    } while (0);
    
    free(pstrNew);
    free(pstrOld);

    return 0LL == nRet;

}
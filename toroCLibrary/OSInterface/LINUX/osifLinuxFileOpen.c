//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileOpen

\brief Implementation of the <em>osifLinuxFileOpen</em> function

\file osifLinuxFileOpen.c

*/
//#include <stdio.h>
//#include <string.h>
#include <CdeServices.h>
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
// string.h
//
extern __declspec(dllimport) int strcmp(const char* pszDst, const char* pszSrc);
//
// protptype
//
OSIFFOPEN _osifLinuxFileOpen;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

#pragma warning( disable : 4101 )

#define _O_RDONLY       0x0000  /* open for reading only */
#define _O_WRONLY       0x0001  /* open for writing only */
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_APPEND       0x0008  /* writes done at fEof */

#define _O_CREAT        0x0100  /* create and open file */

/* O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s,
** and <lf> sequences translated to <cr><lf> on write()'s
*/

#define _O_TEXT         0x4000  /* file mode is text (translated) */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */

/* Temporary file bit - file is deleted when last handle is closed */

#define _O_TEMPORARY    0x0040  /* temporary file bit */

/* Non-ANSI names for compatibility */
#define O_RDONLY        _O_RDONLY
#define O_WRONLY        _O_WRONLY
#define O_RDWR          _O_RDWR
#define O_RDWRMSK       (O_RDONLY | O_WRONLY | O_RDWR)
#define O_APPEND        _O_APPEND
#define O_CREAT         _O_CREAT
#define O_TEXT          _O_TEXT
#define O_BINARY        _O_BINARY
#define O_TEMPORARY     _O_TEMPORARY

// r                open text file for reading
// rb               open binary file for reading
// r+               open text file for update (reading and writing)
// r+b or rb+       open binary file for update (reading and writing)

// a                append; open or create text file for writing at end-of-file
// ab               append; open or create binary file for writing at end-of-file
// a+               append; open or create text file for update, writing at end-of-file
// a+b or ab+       append; open or create binary file for update, writing at end-of-file

// w                truncate to zero length or create text file for writing
// wb               truncate to zero length or create binary file for writing
// w+               truncate to zero length or create text file for update
// w+b or wb+       truncate to zero length or create binary file for update
#define LINUX_O_RDONLY  0x0
#define LINUX_O_WRONLY  0x1
#define LINUX_O_RDWR    0x2
#define LINUX_O_APPEND  0x400
#define LINUX_O_CREAT   0x40

#define OPEN_EXISTING   LINUX_O_RDONLY
#define CREATE_ALWAYS   LINUX_O_CREAT
#define OPEN_ALWAYS     LINUX_O_CREAT

static struct _tblMode {
    const char* pszMode;
    int openmode;       /* BYTE bmDOSAccType:   000 read only
                                                001 write only
                                                010 read/write */
    unsigned  LinuxModeFlags;
}tblMode[] = {
    //O_RDONLY|O_WRONLY|O_RDWR|    O_APPEND|O_CREAT|   O_TEXT| O_BINARY
    {"r"    ,O_RDONLY   | O_TEXT                                , OPEN_EXISTING},
    {"rb"   ,O_RDONLY   | O_BINARY                              , OPEN_EXISTING},
    {"r+"   ,O_RDWR     | O_TEXT                                , OPEN_EXISTING},
    {"r+b"  ,O_RDWR     | O_BINARY                              , OPEN_EXISTING},
    {"rb+"  ,O_RDWR     | O_BINARY                              , OPEN_EXISTING},
    {"w"    ,O_WRONLY   | O_CREAT   | O_TEXT                    , CREATE_ALWAYS + LINUX_O_WRONLY},
    {"wb"   ,O_WRONLY   | O_CREAT   | O_BINARY                  , CREATE_ALWAYS + LINUX_O_WRONLY},
    {"w+"   ,O_RDWR     | O_CREAT   | O_TEXT                    , CREATE_ALWAYS + LINUX_O_RDWR},
    {"w+b"  ,O_RDWR     | O_CREAT   | O_BINARY                  , CREATE_ALWAYS + LINUX_O_RDWR},
    {"wb+"  ,O_RDWR     | O_CREAT   | O_BINARY                  , CREATE_ALWAYS + LINUX_O_RDWR},
    {"a"    ,O_WRONLY   | O_APPEND  | O_CREAT | O_TEXT          , OPEN_ALWAYS + LINUX_O_WRONLY},
    {"ab"   ,O_WRONLY   | O_APPEND  | O_CREAT | O_BINARY        , OPEN_ALWAYS + LINUX_O_WRONLY},
    {"a+"   ,O_RDWR     | O_APPEND  | O_CREAT | O_TEXT          , OPEN_ALWAYS + LINUX_O_RDWR},
    {"a+b"  ,O_RDWR     | O_APPEND  | O_CREAT | O_BINARY        , OPEN_ALWAYS + LINUX_O_RDWR},
    {"ab+"  ,O_RDWR     | O_APPEND  | O_CREAT | O_BINARY        , OPEN_ALWAYS + LINUX_O_RDWR},
};

CDEFILE* _osifLinuxFileOpen(CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, int fFileExists, CDEFILE* pCdeFile)
{
    int i;
    long long fd64 = -1LL;
    char* pszFileName = malloc(sizeof("") + wcslen(pwcsFileName));

    do {

        wcstombs(pszFileName, pwcsFileName, (size_t)-1);
        //
        // search file attributes
        //
        for (i = 0; i < sizeof(tblMode) / sizeof(tblMode[0]); i++)
            if (0 == strcmp(tblMode[i].pszMode, szModeNoSpace))
                break;

        //CDEMOFINE((MFNERR(i == sizeof(tblMode) / sizeof(tblMode[0])) "Attributes \"%s\" NOT found\n", szModeNoSpace));

        if (i == sizeof(tblMode) / sizeof(tblMode[0])) {
            pCdeFile = NULL;
            break;
        }

        //CDEMOFINE((MFNINF(1) "Attributes \"%s\" found, dwCreationDisposition == %08X\n", szModeNoSpace, tblMode[i].LinuxModeFlags));

        fd64 = _syscall(2, (int64_t)pszFileName, tblMode[i].LinuxModeFlags, /*S_IRWXU*/  00700, 0, 0, 0);

        if (fd64 >= 0)
        {
            pCdeFile->emufp = (void*)fd64;
            pCdeFile->openmode = tblMode[i].openmode;
        }
        else
            pCdeFile->emufp = (void*)0LL;

    } while (0);

    free(pszFileName);

    return pCdeFile->emufp;
}
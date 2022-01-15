/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileOpen.c

Abstract:

    OS interface (osif) open a file UEFI Shell

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <windows.h>
#include <cde.h>


#pragma warning( disable : 4101 )

#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void

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

typedef struct tagCDEFILE {
    unsigned char fRsv;                       // 0 if free, 1 if taken /reserved / occupied / allocated
    int     openmode;
    void* emufp;                     // true FILE fp used for emulation in Windows. In true EFI it is the CDEFILE fp
    //long   fpos;                      // fpos -> what ftell returns relative to SEEK_SET
    char* Buffer;                    // internal buffer
    long    bsiz;                       // sizeof internal buffer
    fpos_t  bpos;                       // buffers position equivalent to file pointer position. bpos[63] == SEEK_END marker CDE_FPOS_SEEKEND!!!
    long    bidx;                       // index into Buffer[] ranges from 0 .. bufsiz
    long    bvld;                       // number of valid bytes in the buffer beginning from
    unsigned char bdirty;               // buffer is dirty -> the buffer conatains unwritten characters
    unsigned char bclean;               // buffer is clean -> the buffer conatains characters read from the stream
        //NOTE: dirty and clean TRUE together, implements the microsoft strategy for common read/write data held in a buffer
        //      If CLEAN AND DIRTY are set simoultanously, file writes inhibited
    unsigned char fEof;                        // EOF flag for file
    unsigned char fErr;                        // ERR flag for file
    void* pRootProtocol;
    void* pFileProtocol;
    void* pwcsFileDrive;
    void* pwcsFilePath;
    void* pwcsFileName;
}CDEFILE;

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

static struct _tblMode {
    const char* pszMode;
    int openmode;       /* BYTE bmDOSAccType:   000 read only
                                                001 write only
                                                010 read/write */
    unsigned  WinNTModeFlags;
}tblMode[] = {
        //O_RDONLY|O_WRONLY|O_RDWR|O_APPEND|O_CREAT|O_TEXT| O_BINARY
{"r"    ,O_RDONLY|                                  O_TEXT         , OPEN_EXISTING},
{"rb"   ,O_RDONLY|                                         O_BINARY, OPEN_EXISTING},
{"r+"   ,                   O_RDWR|                 O_TEXT         , OPEN_EXISTING},
{"r+b"  ,                   O_RDWR|                        O_BINARY, OPEN_EXISTING},
{"rb+"  ,                   O_RDWR|                        O_BINARY, OPEN_EXISTING},
{"w"    ,          O_WRONLY|                O_CREAT|O_TEXT         , CREATE_ALWAYS},
{"wb"   ,          O_WRONLY|                O_CREAT|       O_BINARY, CREATE_ALWAYS},    /*NOTE: WinNtSimpleFileSystemOpen does not support EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE only*/
{"w+"   ,                   O_RDWR|         O_CREAT|O_TEXT         , CREATE_ALWAYS},
{"w+b"  ,                   O_RDWR|         O_CREAT|       O_BINARY, CREATE_ALWAYS},
{"wb+"  ,                   O_RDWR|         O_CREAT|       O_BINARY, CREATE_ALWAYS},
{"a"    ,          O_WRONLY|       O_APPEND|O_CREAT|O_TEXT         , OPEN_ALWAYS  },
{"ab"   ,          O_WRONLY|       O_APPEND|O_CREAT|       O_BINARY, OPEN_ALWAYS  },    /*NOTE: WinNtSimpleFileSystemOpen does not support EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE only*/
{"a+"   ,                   O_RDWR|O_APPEND|O_CREAT|O_TEXT         , OPEN_ALWAYS  },
{"a+b"  ,                   O_RDWR|O_APPEND|O_CREAT|       O_BINARY, OPEN_ALWAYS  },
{"ab+"  ,                   O_RDWR|O_APPEND|O_CREAT|       O_BINARY, OPEN_ALWAYS  }
};

/**
Synopsis
    #include <CdeServices.h>
    CDEFILE* _osifWinNTFileOpen(CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, CDEFILE* pCdeFile);
Description
    Open a file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    const wchar_t* pwcsFileName : file name as wide string
    const char* szModeNoSpace   : mode string w/o spaces
    CDEFILE* pCdeFile           : pointer to CDEFILE structure
Returns
    CDEFILE*: success
    NULL    : failure
**/
CDEFILE* _osifWinNTFileOpen(CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, CDEFILE* pCdeFile)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    int i;
    BOOL f;
    const int _TRUE_ = 1, _FALSE_ = 0;

    do {

        // ----- search file attributes

        for (i = 0; i < sizeof(tblMode) / sizeof(tblMode[0]); i++)
            if (0 == strcmp(tblMode[i].pszMode, szModeNoSpace))
                break;


        CDEMOFINE((MFNERR(i == sizeof(tblMode) / sizeof(tblMode[0])) "Attributes \"%s\" NOT found\n", szModeNoSpace));

        if (i == sizeof(tblMode) / sizeof(tblMode[0]))
            break;

        CDEMOFINE((MFNINF(1) "Attributes \"%s\" found, dwCreationDisposition == %08X\n", szModeNoSpace, tblMode[i].WinNTModeFlags));

        hFile = CreateFileW(
            pwcsFileName,               /*__in          LPCTSTR lpFileName,*/
            GENERIC_READ | GENERIC_WRITE,/*__in          DWORD dwDesiredAccess,*/
            FILE_SHARE_WRITE | \
            FILE_SHARE_READ | \
            FILE_SHARE_DELETE,          /*__in          DWORD dwShareMode, set all to enable reopeneing of the same file in a child process for /log writing*/
            0,                          /*__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,*/
            tblMode[i].WinNTModeFlags,  /*__in          DWORD dwCreationDisposition,*/
            FILE_ATTRIBUTE_NORMAL,      /*__in          DWORD dwFlagsAndAttributes,*/
            0                           /*__in          HANDLE hTemplateFile*/
        );

        CDEMOFINE((MFNINF(1) "hFile == %08lX\n", hFile));
        CDEMOFINE((MFNERR(hFile == INVALID_HANDLE_VALUE) "Opening  \"%S\" failed, INVALID_HANDLE_VALUE == %08lX\n", pwcsFileName, INVALID_HANDLE_VALUE));

        //        if(hFile == INVALID_HANDLE_VALUE){
        //            LPVOID lpMsgBuf;
        //
        //                FormatMessage(
        //                        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        //                        FORMAT_MESSAGE_FROM_SYSTEM |
        //                        FORMAT_MESSAGE_IGNORE_INSERTS,
        //                        NULL,
        //                        GetLastError(),
        //                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        //                        (LPTSTR) &lpMsgBuf,
        //                        0, NULL );
        //
        //                CDEMOFINE((MFNERR(1) "GetLastError() -> %s\n",lpMsgBuf));
        //        }

        if (hFile == INVALID_HANDLE_VALUE)
            break;
        if (_TRUE_) {
            pCdeFile->emufp = (void*)hFile;
            pCdeFile->openmode = tblMode[i].openmode;
        }

    } while (_FALSE_);

    return hFile == INVALID_HANDLE_VALUE ? NULL : pCdeFile->emufp;
}


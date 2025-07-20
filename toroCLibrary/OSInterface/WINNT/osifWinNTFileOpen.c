/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTFileOpen.c

Abstract:

    OS interface (osif) open a file UEFI Shell

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
#include <windows.h>

#include <cde.h>
#include <CdeServices.h>

#pragma warning( disable : 4101 )

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
CDEFILE* _osifWinNTFileOpen(CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, int fFileExists/* 0 no, 1 yes, -1 unk */, CDEFILE* pCdeFile)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    unsigned i;
    BOOL f;
    const int _TRUE_ = 1, _FALSE_ = 0;
    unsigned WinNTModeFlags = 0, *pWinNTModeFlags = &WinNTModeFlags;
    int OpenMode = 0, *pOpenMode = &OpenMode;
    uint32_t dwDesiredAccess;

    do {
        //
        // search file attributes
        //
        for (i = 0; i < sizeof(tblMode) / sizeof(tblMode[0]); i++)
            if (0 == strcmp(tblMode[i].pszMode, szModeNoSpace))
                break;

        CDETRACE((CDEERR(i == sizeof(tblMode) / sizeof(tblMode[0])) "Attributes \"%s\" NOT found\n", szModeNoSpace));

        if (i == sizeof(tblMode) / sizeof(tblMode[0])) {
            //
            // reached end of table, check for open() backdoor signature "ctrwaxb"
            //
            if (0 != _stricmp("ctrwaxb", szModeNoSpace))
                break;
            //
            // open() related handling -- NOT related to fopen()
            //
            OpenMode |= O_CREAT   * ('C' == szModeNoSpace[0]);  // get 'c' in "ctrwaxb"
            OpenMode |= O_TRUNC   * ('T' == szModeNoSpace[1]);  // get 't' in "ctrwaxb"
            OpenMode |= O_RDWR    * ('R' == szModeNoSpace[2]);  // get 'r' in "ctrwaxb"
            OpenMode |= O_WRONLY  * ('W' == szModeNoSpace[3]);  // get 'w' in "ctrwaxb"
            OpenMode |= O_APPEND  * ('A' == szModeNoSpace[4]);  // get 'a' in "ctrwaxb"
            OpenMode |= O_TEXT    * ('X' == szModeNoSpace[5]);  // get 'x' in "ctrwaxb"
            OpenMode |= O_BINARY  * ('B' == szModeNoSpace[6]);  // get 'b' in "ctrwaxb"

            if (0 == fFileExists)
            {
                WinNTModeFlags |= OPEN_ALWAYS * ((O_CREAT) == ((O_CREAT)&OpenMode));
                WinNTModeFlags |= OPEN_EXISTING * (0 == ((O_CREAT + O_TRUNC) & OpenMode));
                WinNTModeFlags |= OPEN_ALWAYS * ((O_CREAT + O_APPEND) == ((O_CREAT + O_APPEND) & OpenMode));
            }
            else {
                WinNTModeFlags |= OPEN_ALWAYS   * ((O_CREAT          )  == ((O_CREAT + O_TRUNC)  & OpenMode));    // CREATE_ALWAYS * 0 / * 1
                WinNTModeFlags |= CREATE_ALWAYS * ((          O_TRUNC)  == ((          O_TRUNC)  & OpenMode));    // CREATE_ALWAYS * 0 / * 1
                WinNTModeFlags |= CREATE_ALWAYS * ((O_CREAT + O_TRUNC)  == ((O_CREAT + O_TRUNC)  & OpenMode));    // CREATE_ALWAYS * 0 / * 1
                WinNTModeFlags |= OPEN_EXISTING * (0 == ((O_CREAT + O_TRUNC) & OpenMode));                        // OPEN_EXISTING * 0 / * 1
            }

        }
        else {
            pWinNTModeFlags = &tblMode[i].WinNTModeFlags;       // pointer to fopen() related WinNTModeFlags
            pOpenMode = &tblMode[i].openmode;                   // pointer to fopen() related openmode
        }

        CDETRACE((CDEINF(1) "Attributes \"%s\" found, dwCreationDisposition == %08X\n", szModeNoSpace, tblMode[i].WinNTModeFlags));

        //
        // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew#parameters
        //
        dwDesiredAccess = GENERIC_READ + GENERIC_WRITE * (O_RDONLY != (*pOpenMode & (O_RDONLY | O_WRONLY | O_RDWR)));

        hFile = CreateFileW(
            pwcsFileName,               /*__in          LPCTSTR lpFileName,*/
            dwDesiredAccess,            /*__in          O_RDONLY -> GENERIC_READ, otherwise GENERIC_READ + GENERIC_WRITE*/
            FILE_SHARE_WRITE | \
            FILE_SHARE_READ | \
            FILE_SHARE_DELETE,          /*__in          DWORD dwShareMode, set all to enable reopeneing of the same file in a child process for /log writing*/
            0,                          /*__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,*/
            *pWinNTModeFlags,           /*__in          DWORD dwCreationDisposition,*/
            FILE_ATTRIBUTE_NORMAL,      /*__in          DWORD dwFlagsAndAttributes,*/
            0                           /*__in          HANDLE hTemplateFile*/
        );

        CDETRACE((CDEINF(1) "hFile == %p\n", hFile));
        CDETRACE((CDEERR(hFile == INVALID_HANDLE_VALUE) "Opening  \"%S\" failed, INVALID_HANDLE_VALUE == %p\n", pwcsFileName, INVALID_HANDLE_VALUE));

        if (hFile == INVALID_HANDLE_VALUE) {
            LPVOID lpMsgBuf;
            unsigned err = 0;

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                err = GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0, NULL);

            CDETRACE((CDEERR(1) "GetLastError() %d -> %s\n", err, (char*)lpMsgBuf));
        }

        if (hFile == INVALID_HANDLE_VALUE)
        {
            int e = GetLastError();
            // GetLastError() 5 ERROR_ACCESS_DENIED  -> Access is denied
            // GetLastError() 2 ERROR_FILE_NOT_FOUND -> The system cannot find the file specified.
            // GetLastError() 3 ERROR_PATH_NOT_FOUND
            // TODO add write protection
            // FILE 0000000000000000, lasterror 2->No such file or directory
            // FILE 00000255438A0C40, lasterror 0->No error
            // FILE 0000000000000000, lasterror D->Permission denied
            switch (e)
            {
                case ERROR_ACCESS_DENIED:   pCdeAppIf->nErrno = 13;  /* Permission denied */
                    break;
                case ERROR_FILE_NOT_FOUND:  pCdeAppIf->nErrno = 2;   /* No such file or directory */
                    break;
                default:
                    break;
            }
            break;
        }

        if (_TRUE_) {
            pCdeFile->emufp = (void*)hFile;
            pCdeFile->openmode = *pOpenMode;
        }

    } while (_FALSE_);

    return hFile == INVALID_HANDLE_VALUE ? NULL : pCdeFile->emufp;
}


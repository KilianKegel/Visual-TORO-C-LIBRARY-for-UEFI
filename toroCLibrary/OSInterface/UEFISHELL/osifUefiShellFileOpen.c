/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileOpen.c

Abstract:

    OS interface (osif) open a file UEFI Shell

Author:

    Kilian Kegel

--*/
//#undef NCDETRACE
#define OS_EFI
#include <stddef.h>
#include <limits.h>
//
// string.h
//
extern __declspec(dllimport) wchar_t* wcschr(const wchar_t* wcs, wchar_t c);
extern __declspec(dllimport) wchar_t* wcscat(wchar_t* s1, const wchar_t* s2);
extern __declspec(dllimport) wchar_t* wcscpy(wchar_t* pszDst, const wchar_t* pszSrc);
extern __declspec(dllimport) int wcsncmp(const wchar_t* pszDst, const wchar_t* pszSrc, size_t count);
extern __declspec(dllimport) wchar_t* wcsncpy(wchar_t* pszDst, const wchar_t* pszSrc, size_t n);
extern __declspec(dllimport) wchar_t* wcstok(wchar_t* pszStr, const wchar_t* pszSet, wchar_t** ppLast);
extern __declspec(dllimport) int _wcsicmp(const wchar_t* pszDst, const wchar_t* pszSrc);
extern __declspec(dllimport) int strcmp(const char* pszDst, const char* pszSrc);
extern __declspec(dllimport) int _stricmp(const char* pszDst, const char* pszSrc);
#include <errno.h>

#undef NULL
#include <uefi.h>
#include <Protocol\SimpleFileSystem.h>
#include <Protocol\Shell.h>
#include "Protocol\DevicePathToText.h"

#include <cde.h>
#include <CdeServices.h>

extern void* _CdeLocateProtocol(IN EFI_GUID* Protocol, IN void* Registration OPTIONAL/*,OUT void **Interface*/);

extern EFI_STATUS _CdeLocateHandleBuffer(IN EFI_LOCATE_SEARCH_TYPE SearchType, IN EFI_GUID* Protocol OPTIONAL, IN void* SearchKey OPTIONAL, IN OUT UINTN* NoHandles, OUT EFI_HANDLE** Buffer CDE_OPTIONAL);
extern CHAR16* _cdeConvertDevicePathToText(IN const EFI_DEVICE_PATH_PROTOCOL* DevicePath, IN unsigned char DisplayOnly, IN unsigned char AllowShortcuts);
extern wchar_t* _CdeGetMapFromDevicePath(IN CDE_APP_IF* pCdeAppIf, IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath);
extern const short* _CdeGetCurDir(IN CDE_APP_IF* pCdeAppIf, IN const wchar_t* FileSystemMapping);
extern  EFI_GUID _gEfiSimpleFileSystemProtocolGuid, _gEfiDevicePathToTextProtocolGuid, _gEfiDevicePathProtocolGuid;
extern  EFI_BOOT_SERVICES* _cdegBS;                              // Pointer to boot services

//
// ----- globals
//

EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* pCdeEfiDevicePathToTextProtocol;

//
// ----- definitions for file i/o
//
static struct _tblMode {
    const char* pszMode;
    int openmode;
    //        BYTE bmDOSAccType;  //  000 read only
    //                            //  001 write only
    //                            //  010 read/write
    uint64_t UefiModeFlags;
    uint64_t UefiAttribFlags;

}tblMode[] = {
//           O_RDONLY   O_APPEND    O_CREAT     O_TEXT
//           O_WRONLY                           O_BINARY
//           O_RDWR|                    
    {"r"        ,O_RDONLY |                         O_TEXT      ,EFI_FILE_MODE_READ                                              , EFI_FILE_ARCHIVE},
    {"rb"       ,O_RDONLY |                         O_BINARY    ,EFI_FILE_MODE_READ                                              , EFI_FILE_ARCHIVE},
    {"r+"       ,O_RDWR   |                         O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE                        , EFI_FILE_ARCHIVE},
    {"r+b"      ,O_RDWR   |                         O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE                        , EFI_FILE_ARCHIVE},
    {"rb+"      ,O_RDWR   |                         O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE                        , EFI_FILE_ARCHIVE},
    {"w"        ,O_WRONLY | O_TRUNC  |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"wb"       ,O_WRONLY | O_TRUNC  |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE}, /*NOTE: WinNtSimpleFileSystemOpen does not support EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE only*/
    {"w+"       ,O_RDWR   | O_TRUNC  |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"w+b"      ,O_RDWR   | O_TRUNC  |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"wb+"      ,O_RDWR   | O_TRUNC  |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"a"        ,O_WRONLY | O_APPEND |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"ab"       ,O_WRONLY | O_APPEND |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE}, /*NOTE: WinNtSimpleFileSystemOpen does not support EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE only*/
    {"a+"       ,O_RDWR   | O_APPEND |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"a+b"      ,O_RDWR   | O_APPEND |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"ab+"      ,O_RDWR   | O_APPEND |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
    {"CdeMkDir" ,O_WRONLY | O_APPEND |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE | EFI_FILE_DIRECTORY},
};

static EFI_STATUS efifopen(const char* szModeNoSpace, int fFileExists/* 0 no, 1 yes, -1 unk */, CDEFILE* pCdeFile)
{
    unsigned i;
    EFI_STATUS Status = (EFI_STATUS)-1;
    int OpenMode = 0, * pOpenMode = &OpenMode;
    uint64_t UefiAttribFlags = EFI_FILE_ARCHIVE, * pUefiAttribFlags = &UefiAttribFlags;
    uint64_t UefiModeFlags = 0, * pUefiModeFlags = &UefiModeFlags;

    do {

        // ----- search file attributes

        for (i = 0; i < sizeof(tblMode) / sizeof(tblMode[0]); i++)
            if (0 == strcmp(tblMode[i].pszMode, szModeNoSpace))
                break;


        if (i == sizeof(tblMode) / sizeof(tblMode[0]))
        {
            //
            // reached end of table, check for open() backdoor signature "ctrwaxb"
            //
            if (0 != _stricmp("ctrwaxb", szModeNoSpace))
                break;
            //
            // open() related handling -- NOT related to fopen()
            //
            OpenMode |= O_CREAT  * ('C' == szModeNoSpace[0]);  // get 'c' in "ctrwaxb"
            OpenMode |= O_TRUNC  * ('T' == szModeNoSpace[1]);  // get 't' in "ctrwaxb"
            OpenMode |= O_RDWR   * ('R' == szModeNoSpace[2]);  // get 'r' in "ctrwaxb"
            OpenMode |= O_WRONLY * ('W' == szModeNoSpace[3]);  // get 'w' in "ctrwaxb"
            OpenMode |= O_APPEND * ('A' == szModeNoSpace[4]);  // get 'a' in "ctrwaxb"
            OpenMode |= O_TEXT   * ('X' == szModeNoSpace[5]);  // get 'x' in "ctrwaxb"
            OpenMode |= O_BINARY * ('B' == szModeNoSpace[6]);  // get 'b' in "ctrwaxb"

            if (0 == fFileExists)
            {
                UefiModeFlags |= (EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE) * ((O_CREAT) == ((O_CREAT) & OpenMode));
                //UefiModeFlags |= (EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE) * (0 == ((O_CREAT + O_TRUNC) & OpenMode));
                //UefiModeFlags |= (EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE) * ((O_CREAT + O_APPEND) == ((O_CREAT + O_APPEND) & OpenMode));
            }
            else {
                UefiModeFlags  = (EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);
                UefiModeFlags |= (EFI_FILE_MODE_CREATE) * ((O_CREAT) == ((O_CREAT) & OpenMode));
                UefiModeFlags |= (EFI_FILE_MODE_CREATE) * ((O_TRUNC) == ((O_TRUNC) & OpenMode));
            }
        }
        else
        {
            pUefiAttribFlags = &tblMode[i].UefiAttribFlags;             // pointer to fopen() related UefiAttribFlags
            pUefiModeFlags = &tblMode[i].UefiModeFlags;                 // pointer to fopen() related openmode
            pOpenMode = &tblMode[i].openmode;
        }

        pCdeFile->openmode = *pOpenMode;

        //
        // _open()/fopen() the file
        //
        while (1) {
            Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Open(
                pCdeFile->pRootProtocol,
                &pCdeFile->pFileProtocol,
                pCdeFile->pwcsFilePath,
                *pUefiModeFlags,
                *pUefiAttribFlags
            ));

            //
            // for _open() on w/o files check retry for that particular settings:
            //  1. Status == RETURN_ACCESS_DENIED (file is write protected)
            //  2. szModeNoSpace "?trw??" within "ctrwaxb"
            //  In such case retry with (EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE) cleared
            //
            if (Status == RETURN_ACCESS_DENIED \
                && 't' == szModeNoSpace[1]\
                && 'r' == szModeNoSpace[2]\
                && 'w' == szModeNoSpace[3]\
                && 0 != ((EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE) & *pUefiModeFlags))
            {
                *pUefiModeFlags &= ~(EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE);
            }
            else
            {
                break;
            }

        }

        if (EFI_SUCCESS != Status) 
            break;

        if (O_TRUNC != (O_TRUNC & *pOpenMode))                              // for append, dont't truncate to zero!!!
            break;

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Delete(pCdeFile->pFileProtocol));   // truncate to zero by delete ...

        if (EFI_SUCCESS != Status) 
            break;

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Open(
            pCdeFile->pRootProtocol, 
            &pCdeFile->pFileProtocol, 
            pCdeFile->pwcsFilePath, 
            *pUefiModeFlags, 
            *pUefiAttribFlags
        )); // ... and recreate

    } while (0);

    return Status;  // return Status instead od NULL/pCdeFile to return last Status code to the caller
}

static unsigned char __CdeIsFsEnum(CDE_APP_IF *pCdeAppIf) {
    return pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount != SIZE_MAX ? TRUE : FALSE;
}

static EFI_STATUS __CdeFsEnum(CDE_APP_IF *pCdeAppIf) {
    EFI_STATUS Status = EFI_NOT_READY;
    CDEFSVOLUME* pFsVolume;
    unsigned i;
    void* pTemp;
    wchar_t* pwcsTmp;
    CDETRACE((CDEINF(1)"-->\n"));
    do {


        if (NULL == (pCdeEfiDevicePathToTextProtocol = _CdeLocateProtocol(&_gEfiDevicePathToTextProtocolGuid, NULL))) {
            CDETRACE((CDEINF(1)"-->ERROR\n"));
            break;
        }
        //TODO: ASSERT on Error

// ----- get all handles with EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
        if (1) 
        {
            EFI_STATUS stat;

            CDETRACE((CDEINF(1)"--> nVolumeCount %zd\n", pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount));
            CDETRACE((CDEINF(1)"--> pCdeAppIf->pCdeServices->pCdeSystemVolumes %p\n", pCdeAppIf->pCdeServices->pCdeSystemVolumes));
            CDETRACE((CDEINF(1)"--> pCdeAppIf->pCdeServices %p\n", pCdeAppIf->pCdeServices));
            CDETRACE((CDEINF(1)"--> pCdeAppIf %p\n", pCdeAppIf));

            if (EFI_SUCCESS != (stat = _CdeLocateHandleBuffer(ByProtocol, &_gEfiSimpleFileSystemProtocolGuid, NULL, &pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount, (EFI_HANDLE**)&pTemp)))
            {
                CDETRACE((CDEINF(1)"-->ERROR, status \"%s\"\n", _strefierror(stat)));
                break;
            }
        }
        //            gCdeSystemVolumes.pFsVolume = malloc(gCdeSystemVolumes.nVolumeCount * sizeof(FSVOLUME));

        pFsVolume = &pCdeAppIf->pCdeServices->pCdeSystemVolumes->rgFsVolume[0];

        // ----- for each handle with EFI_SIMPLE_FILE_SYSTEM_PROTOCOL ...

        for (i = 0; i < pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount; i++) {
            
            CDETRACE((CDEINF(1)"--> %02d: nVolumeCount %zd\n", i, pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount));

            // ----- 1. save that particular EFI_HANDLE

            pFsVolume[i].hSimpleFileSystem = ((EFI_HANDLE**)pTemp)[i];

            // ----- 2. get the EFI_SIMPLE_FILE_SYSTEM_PROTOCOL

            Status = _cdegBS->HandleProtocol(pFsVolume[i].hSimpleFileSystem, &_gEfiSimpleFileSystemProtocolGuid, (void**)&pFsVolume[i].pSimpleFileSystemProtocol);//CDEFAULT((CDEFINE_ERRORSTATUS "%s\n",strefierror(Status)));
            CDETRACE((CDEINF(1)"--> %02d: Status \"%s\"\n", i, _strefierror(Status)));

// ----- 3. open the volume / get the EFI_FILE_PROTOCOL

            Status = pFsVolume[i].pSimpleFileSystemProtocol->OpenVolume(pFsVolume[i].pSimpleFileSystemProtocol, &pFsVolume[i].pRootProtocol);
            CDETRACE((CDEINF(1)"--> %02d: Status \"%s\"\n", i, _strefierror(Status)));
            if (EFI_SUCCESS != Status) {
                pFsVolume[i].pSimpleFileSystemProtocol = NULL;
                continue;
            }

            // ----- 4. get the EFI_DEVICE_PATH_PROTOCOL --> THAT IS THE DEVICE PATH itself

            Status = _cdegBS->HandleProtocol(pFsVolume[i].hSimpleFileSystem, &_gEfiDevicePathProtocolGuid, (void**)&pFsVolume[i].pDevicePathProtocol);
            CDETRACE((CDEINF(1)"--> %02d: Status \"%s\"\n", i, _strefierror(Status)));
            //CDEFAULT((CDEFINE_ERRORSTATUS "%s\n",strefierror(Status)));

            pFsVolume[i].pwcsDevicePathText = _cdeConvertDevicePathToText(/*IN DevicePath*/pFsVolume[i].pDevicePathProtocol,/*IN DisplayOnly*/FALSE,/*IN AllowShortcuts*/ FALSE);
            CDETRACE((CDEINF(1)"--> %02d: DevPath \"%ls\"\n", i, pFsVolume[i].pwcsDevicePathText));
            CDETRACE((CDEINF(1)"--> %02d: pvEfiShellProtocol %p\n", i, pCdeAppIf->pCdeServices->pvEfiShellProtocol));
            
            pFsVolume[i].pwcsMap = _CdeGetMapFromDevicePath(pCdeAppIf, &pFsVolume[i].pDevicePathProtocol);
            CDETRACE((CDEINF(1)"--> %02d: DevPath \"%ls\"\n", i, pFsVolume[i].pwcsMap));

//
// initialize rgpVolumeMap[0] .. rgpVolumeMap[CDE_MAPV_MAX - 1] to pointer to '\0'
//
            for (pFsVolume[i].nVolumeMap = 0; pFsVolume[i].nVolumeMap < CDE_MAPV_MAX /* && argv[argc] != NULL*/; pFsVolume[i].nVolumeMap++) {

                if (NULL == (pFsVolume[i].rgpVolumeMap[pFsVolume[i].nVolumeMap] = wcstok(pFsVolume[i].nVolumeMap == 0 ? pFsVolume[i].pwcsMap : NULL, L";", &pwcsTmp))) {

                    pFsVolume[i].rgpVolumeMap[pFsVolume[i].nVolumeMap] = NULL;
                    break;
                }
            }

        }
    } while (EFI_SUCCESS != (Status = EFI_SUCCESS));

    CDETRACE((CDEINF(1)"--> Status \"%s\"\n", _strefierror(Status)));

    return Status;
}

/**
Synopsis
    #include <CdeServices.h>
    CDEFILE* _osifUefiShellFileOpen(IN CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, CDEFILE* pCdeFile);
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
CDEFILE* _osifUefiShellFileOpen(IN CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, int fFileExists/* 0 no, 1 yes, -1 unk */, CDEFILE* pCdeFile) {

    CDEFILE* pRet = NULL;
    unsigned i, j;
    unsigned char TODO = 1;
    EFI_STATUS Status;
    CDETRACE((CDEINF(1)"-->\n"));

    do {/*1. dowhile(0)*/

        if (!__CdeIsFsEnum(pCdeAppIf)) {

            if (EFI_SUCCESS != __CdeFsEnum(pCdeAppIf)) {
                CDETRACE((CDEINF(1)"-->ERROR\n"));
                break;
            }
        }

        if (TODO) {
            //TODO: add filename separation into drive, (relative) pathname, filename etc.
            //20160806 assumed, that filename ist filename only
            wchar_t* pwcsCurDir, * pwcsCurDir2, * pColon, * pDrive, * pCurPath, * pFilePath, * pwcsTargetDir;
            static wchar_t wcsFilePath[258];
            static wchar_t wcsFileName[258];
            static wchar_t wcsDrive[16];
            static wchar_t wcsDrive2[16];
            unsigned char fIsFileAbs = 0, fIsFileDrv = 0;

            // ----- find current drive / path

                //swprintf(wcsFileName, 258, L"%S", pwcsFileName);
            wcscpy(wcsFileName, pwcsFileName);

            fIsFileDrv = (NULL != (pColon = wcschr(wcsFileName, (wchar_t)':')));    // drive name presence is identified by ':'
            fIsFileAbs = wcsFileName[0] == '\\' || (fIsFileDrv ? pColon[1] == (wchar_t)'\\' : 0);

            pwcsCurDir = (wchar_t*)_CdeGetCurDir(pCdeAppIf, NULL);
            pwcsCurDir2 = NULL;

            if (fIsFileDrv) {                                                       // get current directory of remote drive
                wcsncpy(wcsDrive2, wcsFileName, &pColon[1] - wcsFileName);
                pwcsCurDir2 = (wchar_t*)_CdeGetCurDir(pCdeAppIf, wcsDrive2);
            }

            pwcsTargetDir = fIsFileDrv ? (0 == wcsncmp(wcsFileName, pwcsCurDir, &pColon[1] - wcsFileName) ? pwcsCurDir : pwcsCurDir2) : pwcsCurDir;

            pDrive = fIsFileDrv ? wcsFileName : pwcsTargetDir;                         // getting real drive name of the file
            pColon = wcschr(pDrive, (wchar_t)':');
            wcsncpy(wcsDrive, pDrive, &pColon[1] - pDrive);

            pCurPath = 1 + wcschr(pwcsTargetDir, (wchar_t)':');

            pFilePath = (fIsFileDrv ? 1 + wcschr(wcsFileName, (wchar_t)':') : wcsFileName);

            if (fIsFileAbs) {
                wcscpy(wcsFilePath, pFilePath);
            }
            else {
                wcscpy(wcsFilePath, pCurPath);
                wcscat(wcsFilePath, L"\\");
                wcscat(wcsFilePath, pFilePath);
            }
        //
        // ----- find the volume that drive map is provided in
        //
            for (i = 0; i < pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount; i++) {

                for (j = 0; j < CDE_MAPV_MAX; j++) {
                    if (!_wcsicmp(wcsDrive, pCdeAppIf->pCdeServices->pCdeSystemVolumes->rgFsVolume[i].rgpVolumeMap[j])) {
                        break;
                    }
                }
                if (j != CDE_MAPV_MAX) break; //for(i = 0 ; (i < gCdeSystemVolumes.nVolumeCount)  && (FALSE == fBroken); i++)
            }

            if (i == pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount)
            {
                pCdeAppIf->nErrno = ENOENT;
                break;/*1. dowhile(0)*/ //drive map not found, return
            }

            pCdeFile->pwcsFileDrive = wcsDrive;
            pCdeFile->pwcsFilePath  = wcsFilePath;
            pCdeFile->pRootProtocol = pCdeAppIf->pCdeServices->pCdeSystemVolumes->rgFsVolume[i].pRootProtocol;

            Status = efifopen(szModeNoSpace, fFileExists, pCdeFile);            // get emulation file pointer, that is the Windows FP (CDE4WIN) or pCdeFile or NULL in error case
            
            if (EFI_SUCCESS != Status) {
                pRet = NULL;
                /*
                    todo:
                    ERRNO EACCES 13: Permission denied          write protect
                    ERRNO ENOENT 2 : No such file or directory  drive not present
                */
                pCdeAppIf->nErrno = EFI_ACCESS_DENIED   == Status ? EACCES : pCdeAppIf->nErrno;
                pCdeAppIf->nErrno = EFI_WRITE_PROTECTED == Status ? EACCES : pCdeAppIf->nErrno;
                pCdeAppIf->nErrno = EFI_NOT_FOUND       == Status ? ENOENT : pCdeAppIf->nErrno;
                
                break;
            }
            else {
                pRet = pCdeFile;
            }
        }

    } while (0)/*1. dowhile(0)*/;

    return (void*)pRet;
}
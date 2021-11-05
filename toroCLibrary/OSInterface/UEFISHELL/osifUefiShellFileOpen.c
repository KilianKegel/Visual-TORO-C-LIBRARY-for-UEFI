/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileOpen.c

Abstract:

    OS interface (osif) open a file UEFI Shell

Author:

    Kilian Kegel

--*/
#pragma warning( disable : 4101 )
#define OS_EFI
#include <CdeServices.h>
#include <cde.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <Protocol\SimpleFileSystem.h>
#include <Protocol\Shell.h>
#include "Protocol\DevicePathToText.h"

extern int _wcsicmp(const wchar_t* pszDst, const wchar_t* pszSrc);

//
// ----- prototypes
//
FACCESSMODE rgFACCESSMODE[];

static EFI_GUID _gEfiDevicePathProtocolGuid = { 0x09576E91, 0x6D3F, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } };
static EFI_GUID _gEfiSimpleFileSystemProtocolGuid = { 0x964E5B22, 0x6459, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } };
static EFI_GUID _gEfiDevicePathToTextProtocolGuid = { 0x8B843E20, 0x8132, 0x4852, { 0x90, 0xCC, 0x55, 0x1A, 0x4E, 0x4A, 0x7F, 0x1C } };

static EFI_STATUS efifopen(const char* mode, CDEFILE* pCdeFile)
{
    CDEFILE* pRet = NULL;
    int i;
    EFI_STATUS Status = (EFI_STATUS)-1;
    UINT64 UefiModeFlags;
    do {
        for (i = 0; *rgFACCESSMODE[i].pszMode != '\0'/*End Of Table is "" */; i++) {
            if (!strcmp(&rgFACCESSMODE[i].pszMode[0], mode)) {
                break;
            }
        }

        //CDETRACE((CDEFINE"Mode \'%s\' %s \n", mode, *rgFACCESSMODE[i].pszMode == '\0' ? "NOT found" : "found",i));
        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""Mode \'%s\' %s \n", __LINE__, mode , * rgFACCESSMODE[i].pszMode == '\0' ? "NOT found" : "found");

        if (*rgFACCESSMODE[i].pszMode == '\0') {
            //mode string not supported
            //TODO: add error "errno" here
            break;
        }

        pCdeFile->openmode = rgFACCESSMODE[i].openmode;

        //CDETRACE((CDEFINE"Filname: %S, flags: %016llX\n", pCdeFile->pwcsFilePath,rgFACCESSMODE[i].UefiModeFlags));
        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""Filname: %S, flags: %016llX, Attribs %016llX\n", __LINE__, pCdeFile->pwcsFilePath, rgFACCESSMODE[i].UefiModeFlags, rgFACCESSMODE[i].UefiAttribFlags);

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Open(
            pCdeFile->pRootProtocol, 
            &pCdeFile->pFileProtocol, 
            pCdeFile->pwcsFilePath, 
            rgFACCESSMODE[i].UefiModeFlags, 
            rgFACCESSMODE[i].UefiAttribFlags
        ));
        //CDEFAULT( (CDEFINE_ERRORSTATUS "%s\n",strefierror(efierrno)) );
        
        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""%s\n",__LINE__, strefierror(Status));

        if (EFI_SUCCESS != Status) 
            break;

        if (O_APPEND == (O_APPEND & rgFACCESSMODE[i].openmode))           // for append, dont't truncate to zero!!!
            break;

        if (0 == (EFI_FILE_MODE_CREATE & rgFACCESSMODE[i].UefiModeFlags)) // EFI_FILE_MODE_CREATE set for create or truncate to zero
            break;

        //CDETRACE((CDEFINE"Truncate file \"%S\" to ZERO\n", pCdeFile->pwcsFilePath));
        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""Truncate file \"%S\" to ZERO\n", __LINE__, pCdeFile->pwcsFilePath);

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Delete(pCdeFile->pFileProtocol));   // truncate to zero by delete ...
        //CDEFAULT( (CDEFINE_ERRORSTATUS "%s\n",strefierror(efierrno)) );
        //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""%s\n", __LINE__, strefierror(Status));

        if (EFI_SUCCESS != Status) 
            break;

        Status = __cdeOnErrSet_status(pCdeFile->pRootProtocol->Open(
            pCdeFile->pRootProtocol, 
            &pCdeFile->pFileProtocol, 
            pCdeFile->pwcsFilePath, 
            rgFACCESSMODE[i].UefiModeFlags, 
            rgFACCESSMODE[i].UefiAttribFlags
        )); // ... and recreate

        //CDEFAULT( (CDEFINE_ERRORSTATUS "%s\n",strefierror(efierrno)) );

        if (EFI_SUCCESS != Status)
            break;

        //CDETRACE((CDEFINE"File \"%S\" is SUCESSFULLY opened\n", pCdeFile->pwcsFilePath));

    } while (0);

    //pRet = EFI_SUCCESS == Status ? pCdeFile : NULL;

    //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""Status -> %llX %s\n", __LINE__, Status, strefierror(Status));

    return Status;  // return Status instead od NULL/pCdeFile to return last Status code to the caller
}


//
// ----- externs
//
extern CDESYSTEMVOLUMES gCdeSystemVolumes;
extern void* _CdeLocateProtocol(IN EFI_GUID* Protocol, IN void* Registration OPTIONAL/*,OUT void **Interface*/);

extern EFI_STATUS _CdeLocateHandleBuffer(IN EFI_LOCATE_SEARCH_TYPE SearchType, IN EFI_GUID* Protocol OPTIONAL, IN void* SearchKey OPTIONAL, IN OUT UINTN* NoHandles, OUT EFI_HANDLE** Buffer CDE_OPTIONAL);
extern CHAR16* _cdeConvertDevicePathToText(IN const EFI_DEVICE_PATH_PROTOCOL* DevicePath, IN unsigned char DisplayOnly, IN unsigned char AllowShortcuts);
extern CHAR16* _CdeGetMapFromDevicePath(IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath);
extern const CHAR16* _CdeGetCurDir(IN const CHAR16* FileSystemMapping OPTIONAL);
extern  EFI_SHELL_PROTOCOL* pEfiShellProtocol;
extern  EFI_GUID                _gEfiSimpleFileSystemProtocolGuid;
extern  EFI_BOOT_SERVICES* _cdegBS;                              // Pointer to boot services

//
// ----- globals
//
extern EFI_GUID _gCdeEfiShellProtocolGuid;

FACCESSMODE rgFACCESSMODE[] = {
//           O_RDONLY   O_APPEND    O_CREAT     O_TEXT
//           O_WRONLY                           O_BINARY
//           O_RDWR|                    

{"r"        ,O_RDONLY |                         O_TEXT      ,EFI_FILE_MODE_READ                                              , EFI_FILE_ARCHIVE},
{"rb"       ,O_RDONLY |                         O_BINARY    ,EFI_FILE_MODE_READ                                              , EFI_FILE_ARCHIVE},
{"r+"       ,O_RDWR   |                         O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE                        , EFI_FILE_ARCHIVE},
{"r+b"      ,O_RDWR   |                         O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE                        , EFI_FILE_ARCHIVE},
{"rb+"      ,O_RDWR   |                         O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE                        , EFI_FILE_ARCHIVE},
{"w"        ,O_WRONLY |             O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"wb"       ,O_WRONLY |             O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE}, /*NOTE: WinNtSimpleFileSystemOpen does not support EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE only*/
{"w+"       ,O_RDWR   |             O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"w+b"      ,O_RDWR   |             O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"wb+"      ,O_RDWR   |             O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"a"        ,O_WRONLY | O_APPEND |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"ab"       ,O_WRONLY | O_APPEND |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE}, /*NOTE: WinNtSimpleFileSystemOpen does not support EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE only*/
{"a+"       ,O_RDWR   | O_APPEND |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"a+b"      ,O_RDWR   | O_APPEND |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"ab+"      ,O_RDWR   | O_APPEND |  O_CREAT |   O_BINARY    ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE},
{"CdeMkDir" ,O_WRONLY | O_APPEND |  O_CREAT |   O_TEXT      ,EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE , EFI_FILE_ARCHIVE | EFI_FILE_DIRECTORY},
{""/*EOT*/,0,0} };

EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* pCdeEfiDevicePathToTextProtocol;

static unsigned char __CdeIsFsEnum(void) {
    return gCdeSystemVolumes.nVolumeCount != -1 ? TRUE : FALSE;
}

static EFI_STATUS __CdeFsEnum(void) {
    EFI_STATUS Status = EFI_NOT_READY;
    CDEFSVOLUME* pFsVolume;
    int i;
    void* pTemp;
    wchar_t* pwcsTmp;
    do {


        if (NULL == (pCdeEfiDevicePathToTextProtocol = _CdeLocateProtocol(&_gEfiDevicePathToTextProtocolGuid, NULL))) {
            break;
        }
        //TODO: ASSERT on Error

// ----- get all handles with EFI_SIMPLE_FILE_SYSTEM_PROTOCOL

        if (EFI_SUCCESS != _CdeLocateHandleBuffer(ByProtocol, &_gEfiSimpleFileSystemProtocolGuid, NULL, (UINTN*)&gCdeSystemVolumes.nVolumeCount, (EFI_HANDLE**)&pTemp)) {
            break;
        }
        //            gCdeSystemVolumes.pFsVolume = malloc(gCdeSystemVolumes.nVolumeCount * sizeof(FSVOLUME));

        pFsVolume = &gCdeSystemVolumes.rgFsVolume[0];

        // ----- for each handle with EFI_SIMPLE_FILE_SYSTEM_PROTOCOL ...

        for (i = 0; i < gCdeSystemVolumes.nVolumeCount; i++) {

            // ----- 1. save that particular EFI_HANDLE

            pFsVolume[i].hSimpleFileSystem = ((EFI_HANDLE**)pTemp)[i];

            // ----- 2. get the EFI_SIMPLE_FILE_SYSTEM_PROTOCOL

            Status = _cdegBS->HandleProtocol(pFsVolume[i].hSimpleFileSystem, &_gEfiSimpleFileSystemProtocolGuid, &pFsVolume[i].pSimpleFileSystemProtocol);//CDEFAULT((CDEFINE_ERRORSTATUS "%s\n",strefierror(Status)));

// ----- 3. open the volume / get the EFI_FILE_PROTOCOL

            Status = pFsVolume[i].pSimpleFileSystemProtocol->OpenVolume(pFsVolume[i].pSimpleFileSystemProtocol, &pFsVolume[i].pRootProtocol);
            //CDEFAULT((CDEFINE_WARNSTATUS "%s\n",strefierror(Status)));
            if (EFI_SUCCESS != Status) {
                pFsVolume[i].pSimpleFileSystemProtocol = NULL;
                continue;
            }

            // ----- 4. get the EFI_DEVICE_PATH_PROTOCOL --> THAT IS THE DEVICE PATH itself

            Status = _cdegBS->HandleProtocol(pFsVolume[i].hSimpleFileSystem, &_gEfiDevicePathProtocolGuid, &pFsVolume[i].pDevicePathProtocol);
            //CDEFAULT((CDEFINE_ERRORSTATUS "%s\n",strefierror(Status)));

            pFsVolume[i].pwcsDevicePathText = _cdeConvertDevicePathToText(/*IN DevicePath*/pFsVolume[i].pDevicePathProtocol,/*IN DisplayOnly*/FALSE,/*IN AllowShortcuts*/ FALSE);
            //CDETRACE((CDEFINE"%S \n",pFsVolume[i].pwcsDevicePathText));

            pFsVolume[i].pwcsMap = _CdeGetMapFromDevicePath(&pFsVolume[i].pDevicePathProtocol);
            //CDETRACE((CDEFINE"%S\n",pFsVolume[i].pwcsMap ? pFsVolume[i].pwcsMap : L"leer"));

//
// initialize rgpVolumeMap[0] .. rgpVolumeMap[CDE_MAPV_MAX - 1] to pointer to '\0'
//
            for (pFsVolume[i].nVolumeMap = 0; pFsVolume[i].nVolumeMap < CDE_MAPV_MAX /* && argv[argc] != NULL*/; pFsVolume[i].nVolumeMap++) {

                if (NULL == (pFsVolume[i].rgpVolumeMap[pFsVolume[i].nVolumeMap] = wcstok(pFsVolume[i].nVolumeMap == 0 ? pFsVolume[i].pwcsMap : NULL, L";", &pwcsTmp))) {

                    pFsVolume[i].rgpVolumeMap[pFsVolume[i].nVolumeMap] = NULL;
                    break;
                }
                //CDETRACE((CDEFINE"map[%d]: %S\n", pFsVolume[i].nVolumeMap, pFsVolume[i].rgpVolumeMap[ pFsVolume[i].nVolumeMap]));
            }

        }
    } while (EFI_SUCCESS != (Status = EFI_SUCCESS));

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
CDEFILE* _osifUefiShellFileOpen(IN CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, CDEFILE* pCdeFile) {

    CDEFILE* pRet = NULL;
    int i, j;
    unsigned char TODO = 1;
    EFI_STATUS Status;

    //printf(/*__FILE__*/"(%d), "__FUNCTION__"()"">>>\n", __LINE__);
    
    do {/*1. dowhile(0)*/

        if (!__CdeIsFsEnum()) {

            if (EFI_SUCCESS != __CdeFsEnum()) {
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

            pwcsCurDir = (wchar_t*)_CdeGetCurDir(NULL);
            pwcsCurDir2 = NULL;

            if (fIsFileDrv) {                                                       // get current directory of remote drive
                wcsncpy(wcsDrive2, wcsFileName, &pColon[1] - wcsFileName);
                pwcsCurDir2 = (wchar_t*)_CdeGetCurDir(wcsDrive2);
                //CDETRACE((CDEFINE"wcsDrive2 == %S, pwcsCurDir2 == %S\n", wcsDrive2, pwcsCurDir2));
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
            //CDETRACE((CDEFINE"fIsFileAbs == %d, fIsFileDrv == %d, Drive == %S, wcsFilePath == %S\n", fIsFileAbs, fIsFileDrv, wcsDrive, wcsFilePath));
        //
        // ----- find the volume that drive map is provided in
        //
            for (i = 0; i < gCdeSystemVolumes.nVolumeCount; i++) {

                for (j = 0; j < CDE_MAPV_MAX; j++) {
                    if (!_wcsicmp(wcsDrive, gCdeSystemVolumes.rgFsVolume[i].rgpVolumeMap[j])) {
                        break;
                    }
                }
                if (j != CDE_MAPV_MAX) break; //for(i = 0 ; (i < gCdeSystemVolumes.nVolumeCount)  && (FALSE == fBroken); i++)
            }

            if (i == gCdeSystemVolumes.nVolumeCount) 
            {
                //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""ERROR: drive map not found, gCdeSystemVolumes.nVolumeCount %d\n", __LINE__, (int)gCdeSystemVolumes.nVolumeCount);
                errno = ENOENT;
                break;/*1. dowhile(0)*/ //drive map not found, return
            }

            pCdeFile->pwcsFileDrive = wcsDrive;
            pCdeFile->pwcsFilePath = wcsFilePath;
            pCdeFile->pRootProtocol = gCdeSystemVolumes.rgFsVolume[i].pRootProtocol;

            Status = efifopen(szModeNoSpace, pCdeFile);            // get emulation file pointer, that is the Windows FP (CDE4WIN) or pCdeFile or NULL in error case
            if (EFI_SUCCESS != Status) {
                pRet = NULL;
                //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""%s\n", __LINE__, strefierror(Status));
                /*
                    todo:
                    ERRNO EACCES 13: Permission denied		    write protect
                    ERRNO ENOENT 2 : No such file or directory	drive not present
                */
                errno = EFI_WRITE_PROTECTED == Status ? EACCES : errno;
                //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""%s\n", __LINE__, strerror(errno));
                break;
            }
            else {
                pRet = pCdeFile;
                //printf(/*__FILE__*/"(%d), "__FUNCTION__"()""%s\n", __LINE__, strefierror(Status));
            }
        }

    } while (0)/*1. dowhile(0)*/;

    return (void*)pRet;
}
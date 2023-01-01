/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellFileRename.c

Abstract:

    OS interface (osif) to rename a file UEFI Shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <PiPei.h>
#include <Base.h>
#include <CdeServices.h>
#include <Protocol\Shell.h>
#include <guid\FileInfo.h>
#include <cde.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

#define ELC(x) sizeof(x) / sizeof(x[0])

extern EFI_SHELL_PROTOCOL* pEfiShellProtocol;
extern EFI_SYSTEM_TABLE* _cdegST;
extern CDEFILE* _osifUefiShellFileOpen(IN CDE_APP_IF* pCdeAppIf, const wchar_t* pwcsFileName, const char* szModeNoSpace, CDEFILE* pCdeFile);

/**
Synopsis
    #include <CdeServices.h>
    int _osifUefiShellFileRename(IN CDE_APP_IF* pCdeAppIf, wchar_t* pwcsOldName, wchar_t* pwcsNewName)
Description
    Rename a file
Paramters
    IN CDE_APP_IF* pCdeAppIf    : application interface
    wchar_t* pwcsOldName        : old name wide string
    wchar_t* pwcsNewName        : new name wide string
Returns
    CDEFILE*: success
    NULL    : failure
**/
int _osifUefiShellFileRename(IN CDE_APP_IF* pCdeAppIf, wchar_t* pwcsOldName, wchar_t* pwcsNewName)
{
    size_t FileNameLengthOld = 1 + wcslen(pwcsOldName); // length of old filename
    size_t FileNameLengthNew = 1 + wcslen(pwcsNewName); // length of new filename, in case it is preceeded by a path, just ignore the additional allocated space
                                                        // NOTE: The filename only, w/o leading path, is required by the UEFI-SetInfo() interface
    size_t FileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(wchar_t) * (/*allocate the space for the longer filename old vs. new*/ FileNameLengthNew >= FileNameLengthOld ? FileNameLengthNew : FileNameLengthOld);
    EFI_FILE_INFO* pFileInfo = malloc(FileInfoSize);
    CDEFILE CdeFile, * pCdeFile = memset(&CdeFile, 0, sizeof(CdeFile)); // initialize buffer to ZERO
    EFI_GUID FileInfoGuid = EFI_FILE_INFO_ID;
    EFI_FILE_PROTOCOL* pFileProtocol;                                   // file protocol of the actually opened (old)file
    EFI_STATUS Status;
    int nRet = 0;
    void* pTemp;

    do {
        //
        // open the file using the internal function _osifUefiShellFileOpen(), since file to rename must be open
        //
        pTemp = _osifUefiShellFileOpen(pCdeAppIf, pwcsOldName, "r+", pCdeFile);          // get emulation file pointer, that is the Windows FP (CDE4WIN) or pCdeFile or NULL in error case
        CDEMOFINE((MFNERR(NULL == pTemp) "Can not open file \"%S\"\n", pwcsOldName));
        if (NULL == pTemp) {
            errno = ENOENT; //errno -> 2 == No such file or directory, GetLastError() 2
            break;
        }

        //
        // get FileInfo, buffer with biggest possible filename (old vs. new) is already allocated: space should be sufficent
        //
        pFileProtocol = CdeFile.pFileProtocol;                                                      // there is the FileProtocol to get the FileInfo
        Status = pFileProtocol->GetInfo(pFileProtocol, &FileInfoGuid, &FileInfoSize, pFileInfo);    // detect required buffer size
        CDEMOFINE((MFNERR(EFI_SUCCESS != Status) "Status: %s, FileInfoSize == %d, sizeof(EFI_FILE_INFO) == %d\n", strefierror(Status), FileInfoSize, sizeof(EFI_FILE_INFO)));
        if (EFI_SUCCESS != Status)
            break;

        {
            //
            // assign new name w/o path to the UEFI FileInfo
            //
            wchar_t* pwcsNewNameOnly;

            pwcsNewNameOnly = wcsrchr(pwcsNewName, '\\');   // search backwards backslash for end of path (if path is contained)
            if (NULL != pwcsNewNameOnly)                    // if pwcsNewName contains a path (\) ...
                pwcsNewNameOnly++;                          // ...increment to filename only
            else
                pwcsNewNameOnly = pwcsNewName;              // ... else just use the pwcsNewName

            wcscpy(pFileInfo->FileName, pwcsNewNameOnly);
        }

        FileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(wchar_t) * (1/*termination zero*/ + wcslen(pFileInfo->FileName));
        Status = pFileProtocol->SetInfo(pFileProtocol, &FileInfoGuid, FileInfoSize, pFileInfo);
        CDEMOFINE((MFNERR(EFI_SUCCESS != Status) "Status: %s, FileInfoSize == %d, sizeof(EFI_FILE_INFO) == %d\n", strefierror(Status), FileInfoSize, sizeof(EFI_FILE_INFO)));
        if (EFI_SUCCESS != Status) {
            errno = EEXIST; //errno -> 17 == File exists, GetLastError() 183
            break;
        }

        nRet = 1;

    } while (0);

    free(pFileInfo);

    return nRet;
}
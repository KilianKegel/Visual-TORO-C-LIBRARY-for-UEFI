/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifUefiShellCmdExec.c

Abstract:

    OS interface (osif) to run a command on system shell

Author:

    Kilian Kegel

--*/
#define OS_EFI
#define _INC_STDDEF         // exclude MSFT STDDEF.H, that conflicts with errno
#include <CdeServices.h>
#include <cde.h>
//
// wchar.h
//
extern __declspec(dllimport) int wcsncmp(const wchar_t* pszDst, const wchar_t* pszSrc, size_t count);
extern __declspec(dllimport) wchar_t* wcsstr(const wchar_t* pszStr, const wchar_t* pszSubStr);

//
// string.h
//
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);

#include <Protocol\SimpleFileSystem.h>
#include <CdeServices.h>
#include <Protocol\Shell.h>
#include "Protocol\DevicePathToText.h"
# define ELC(x)/*ELementCount*/ (sizeof(x) / sizeof(x[0]))

extern EFI_SYSTEM_TABLE* _cdegST;

EFI_TEXT_CLEAR_SCREEN   pConIOClr;
EFI_TEXT_STRING         pConIOPutStr;
EFI_FILE_WRITE          pEFI_FILE_WRITE;
EFI_ALLOCATE_POOL       pAllocPool;
EFI_IMAGE_START         pStartImage;

static EFI_STATUS EFIAPI myConIOClr(IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This) {

    return EFI_SUCCESS;
}


static enum STATE {
    AWAITING_UEFISHELL1,/*   checked: L"UEFI Interactive Shell v"           */
    AWAITING_UEFISHELL2,/*notchecked: L"UDK2018..."                         */
    AWAITING_UEFISHELL3,/*notchecked: L"UEFI v2."                           */
    AWAITING_MAPPINGTBL,/*   checked: L"Mapping tabl"                       */
    AWAITING_ALIAS0,    /*   checked: L"Alias(s)"                           */
    AWAITING_ALIAS1,    /*notchecked: L"HD0b0" / L"BLK0:" + L"PciRoot..."   */
    AWAITING_DRIVENAME0,/*   checked: L"     FS" / L"     BLK"              */
    STOP_WAITING,
}state;

static EFI_STATUS EFIAPI myEFI_FILE_WRITE(IN EFI_FILE_PROTOCOL* This, IN OUT UINTN* BufferSize, IN VOID* Buffer)
{
    EFI_STATUS Status = EFI_SUCCESS;
    static wchar_t wcsSHELL[] = { L"UEFI Interactive Shell v" };
    static wchar_t wcsMAP[] = { L"Mapping table" };
    static wchar_t wcsALIAS[] = { L"Alias(s)" };
    static wchar_t wcsBLK[] = { 0x20,0x20,0x20,0x20,0x20,0x42,0x4C,0x4B/*L"BLK"*/,0 };
    static wchar_t wcsBLK2[] = { 0x20,0x20,0x20,0x20,0x42,0x4C,0x4B/*L"BLK"*/,0 };
    static wchar_t wcsFS[] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x46,0x53/*L"FS"*/,0 };
    static wchar_t wcsFS2[] = { 0x20,0x20,0x20,0x20,0x20,0x46,0x53/*L"FS"*/,0 };

    switch (state) {
    case AWAITING_UEFISHELL1:
        if (0 == wcsncmp(wcsSHELL, Buffer, ELC(wcsSHELL) - 1))
            state = AWAITING_UEFISHELL2;
        else
            state = STOP_WAITING;
        break;
    case AWAITING_UEFISHELL2:
        state = AWAITING_UEFISHELL3;
        break;
    case AWAITING_UEFISHELL3:
        state = AWAITING_MAPPINGTBL;
        break;
    case AWAITING_MAPPINGTBL:
        if (0 == wcsncmp(wcsMAP, Buffer, ELC(wcsMAP) - 1))
            state = AWAITING_ALIAS0;
        else
            state = STOP_WAITING;
        break;
    case AWAITING_ALIAS0:
        if (0 == wcsncmp(wcsALIAS, Buffer, ELC(wcsALIAS) - 1))
            state = AWAITING_ALIAS1;
        break;
    case AWAITING_ALIAS1:
        state = AWAITING_DRIVENAME0;
        break;
    case AWAITING_DRIVENAME0:
        char fDOSDRV = 0;
        wchar_t wcsDOSDRV[] = { L"A:" };

        for (wchar_t wc = 'A'; wc < 'Z'; wc++)
        {
            wcsDOSDRV[0] = wc;

            fDOSDRV = (NULL != wcsstr(Buffer, wcsDOSDRV));

            if (fDOSDRV)
                break;
        }
        
        if (1 == fDOSDRV || \
            0 == wcsncmp(wcsBLK, Buffer, ELC(wcsBLK) - 1) || \
            0 == wcsncmp(wcsBLK2, Buffer, ELC(wcsBLK2) - 1) || \
            0 == wcsncmp(wcsFS, Buffer, ELC(wcsFS) - 1) || \
            0 == wcsncmp(wcsFS2, Buffer, ELC(wcsFS2) - 1))
        {
            state = AWAITING_ALIAS0;
            //fDOSDRV = 0;
        }
        else
            state = STOP_WAITING;
        break;
    }

    if (STOP_WAITING == state) {
        Status = pEFI_FILE_WRITE(This, BufferSize, Buffer);
    }

    return Status;
}



/** _osifCmdExec() - run command on system shell

Synopsis
    #include <CdeServices.h>
    int _osifUefiShellCmdExec(CDE_APP_IF* pCdeAppIf, char* szCommand);
Description
    Run command on system shell
Parameters
    CDE_APP_IF* pCdeAppIf   - application interface
    char* szCommand         - command to run
Returns
    return value of the command
**/
int _osifUefiShellCmdExec(CDE_APP_IF* pCdeAppIf, char* szCommand) {
    EFI_STATUS nRetStatus = 0;
    EFI_STATUS Status = 0;
    static wchar_t wcsCommand[CDE_SLASHCEXTENDETCOMMAND_LEN];
    size_t i;
    size_t len = strlen(szCommand);
    EFI_SHELL_PROTOCOL* pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;

    do {

        if (CDE_SLASHCEXTENDETCOMMAND_LEN < 1 + len) {

        }

        // ----- convert from char to wchar string

        for (i = 0; i < len + 1; i++)
            wcsCommand[i] = (0xFF) & (wchar_t)szCommand[i];

        if (1) {

            pConIOClr = _cdegST->ConOut->ClearScreen;
            pEFI_FILE_WRITE = CDE_STDOUT->pFileProtocol->Write;

            CDE_STDOUT->pFileProtocol->Write = myEFI_FILE_WRITE;
            _cdegST->ConOut->ClearScreen = myConIOClr;


            state = AWAITING_UEFISHELL1;
        }

        Status = pEfiShellProtocol->Execute(&pCdeAppIf->DriverParm.BsDriverParm.ImageHandle, wcsCommand, NULL, &nRetStatus);

        if (1) {

            _cdegST->ConOut->ClearScreen = pConIOClr;
            CDE_STDOUT->pFileProtocol->Write = pEFI_FILE_WRITE;
        }

    } while (0);

    return (int)nRetStatus;
}
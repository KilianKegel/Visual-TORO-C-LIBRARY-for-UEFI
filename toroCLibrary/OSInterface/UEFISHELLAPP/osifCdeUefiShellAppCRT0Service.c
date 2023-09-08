/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifCdeUefiShellAppCRT0Service.c

Abstract:
    ###################################################################################
    ### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
    ###################################################################################

    CRT0, CdeServices based

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <uefi.h>
#include <cde.h>
//#undef NULL
#include <CdeServices.h>
#include <Protocol\LoadedImage.h>
#include <Protocol\Shell.h>
#include <Protocol\ShellParameters.h>
//
// stdio.h
//
#define EOF    (-1)

extern void _disable(void);
extern void _enable(void);

#pragma intrinsic (_disable, _enable)
extern int main(int argc, char** argv);
extern CRT0SERVICE _osifCdeUefiShellAppCRT0Service;
extern EFI_GUID _gCdeEfiShellProtocolGuid;
extern EFI_GUID _gEfiShellParametersProtocolGuid;
extern char __cdeGetCurrentPrivilegeLevel(void);
extern int _cdeCoreSprintf(CDE_APP_IF* pCdeAppIf, char* pszDest, const char* pszFormat, ...);
extern int _cdeStr2Argcv(char** argv, char* szCmdline);
//extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
//extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
//extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) int fclose(FILE* stream);
//extern __declspec(dllimport) int fgetc(FILE* stream);
extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
//extern __declspec(dllimport) int sprintf(char* pszDest, const char* pszFormat, ...);
//extern __declspec(dllimport) int raise(int sig);

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
static EFI_STATUS EFIAPI _MainEntryPoint(
    IN CDE_APP_IF *pCdeAppIf
)
{
    EFI_HANDLE  ImageHandle = pCdeAppIf->DriverParm.BsDriverParm.ImageHandle;
    EFI_SYSTEM_TABLE* pSystemTable = pCdeAppIf->DriverParm.BsDriverParm.pSystemTable;
    CDE_SERVICES* pCdeServices = pCdeAppIf->pCdeServices;

    //void* argvex[CDE_ARGV_MAX + 2];         // DON'T INITIALIZE HERE, BECAUSE C COMPILER inserts memset()!!!
    static char szDelims[] = { " \t" };
    wchar_t* pwcsCmdLine;
    char szCmdLine[CDE_CMD_LINE_LEN];
    int argc = 1;
    EFI_STATUS Status = EFI_SUCCESS;
    static EFI_GUID EfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_LOADED_IMAGE_PROTOCOL* pLoadedImageProtocol;
    static EFI_GUID EfiSimpleTextOutputProtocolGuid = EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID;
    int i;
    static ROMPARM_VWXPRINTF RomParmVWXPRINTF = { \
        /*fForceToDataSeg       */ 2 ,\
        /*fPointerIsParm        */ 0 ,\
        /*fPointerIsFilePointer */ 0 ,\
        /*fCountIsParm;         */ 0 ,\
        /*fAjustDifference      */ 0 ,\
        /*fWide                 */ 0 ,\
    };
    size_t eflags = __readeflags();
    EFI_SHELL_PROTOCOL* pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;

    CDEPOSTCODE(IS64BITCODE, 0xC0);
    if (1)
        do {


            memset(pCdeAppIf->pArgvEx[0], 0, pCdeAppIf->cArgvEx * sizeof(void*));           // instead do dedicated initialization that invokes __imp__memset()
            pCdeAppIf->pArgvEx[0] = (void*)ImageHandle;                                     // instead do dedicated initialization that invokes __imp__memset()
            pCdeAppIf->pArgvEx[1] = (void*)pSystemTable;                                    // instead do dedicated initialization that invokes __imp__memset()

            //
            // check shell version
            //

            //if (NULL == (pvEfiShellProtocol = _CdeLocateProtocol(&_gCdeEfiShellProtocolGuid, NULL))) {
            //    pCdeST->ConOut->OutputString(pCdeST->ConOut, L"FATAL ERROR: Shell Protocol missing\r\nDownload UEFI SHELL from:\r\n\thttps://github.com/tianocore/edk2/blob/master/ShellBinPkg/UefiShell/X64/Shell.efi\r\nand copy to EFI\\BOOT\\BOOTX64.EFI");
            //    Status = EFI_INCOMPATIBLE_VERSION;
            //    break;
            //}

            //
            // check shell version
            //
            if (1) {
                int ShellVerNum = 0;

                if (NULL != pEfiShellProtocol)
                {
                    ShellVerNum = 100 * pEfiShellProtocol->MajorVersion;
                    ShellVerNum += pEfiShellProtocol->MinorVersion;
                }

                if (202 > ShellVerNum) {
                    pSystemTable->ConOut->OutputString(pSystemTable->ConOut, L"FATAL ERROR: Shell Protocol missing or below v2.2\r\nDownload UEFI SHELL from:\r\n    https://github.com/tianocore/edk2/blob/edk2-stable201903/ShellBinPkg/UefiShell/X64/Shell.efi\r\nand copy to EFI\\BOOT\\BOOTX64.EFI\r\n");
                    Status = EFI_INCOMPATIBLE_VERSION;
                    break;
                }
            }

            Status = pSystemTable->BootServices->HandleProtocol(ImageHandle, &EfiLoadedImageProtocolGuid, &pLoadedImageProtocol);

            if (1/* setup file i/o, stdin, stout, stderr*/) {
                UINT64 Position;
                EFI_SHELL_PARAMETERS_PROTOCOL* pEfiShellParametersProtocol;

                memset(pCdeServices->pCdeSystemVolumes, 0, sizeof(CDESYSTEMVOLUMES));   // clear entire structure
                pCdeServices->pCdeSystemVolumes->nVolumeCount = (UINTN) -1;            // mark volume enumeration not yet done

                memset(pCdeAppIf->pIob, 0, sizeof(CDEFILE) * pCdeAppIf->cIob);          // clear entire structure

                Status = pSystemTable->BootServices->HandleProtocol(ImageHandle, &_gEfiShellParametersProtocolGuid, &pEfiShellParametersProtocol);

                CDE_STDIN->pRootProtocol = pEfiShellParametersProtocol->StdIn;

                CDE_STDIN->pFileProtocol = pEfiShellParametersProtocol->StdIn;
                CDE_STDIN->fRsv = TRUE;
                CDE_STDIN->openmode = O_TEXT + O_RDONLY + O_CDESTDIN + O_CDENOSEEK + (EFI_UNSUPPORTED == pCdeAppIf->pIob[0].pFileProtocol->GetPosition(pCdeAppIf->pIob[0].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
                CDE_STDIN->emufp = &pCdeAppIf->pIob[0];

                CDE_STDOUT->pRootProtocol = pEfiShellParametersProtocol->StdOut;
                CDE_STDOUT->pFileProtocol = pEfiShellParametersProtocol->StdOut;
                CDE_STDOUT->fRsv = TRUE;
                CDE_STDOUT->openmode = O_TEXT + O_WRONLY + O_CDESTDOUT + (pCdeAppIf->STDOUT816BitMode == 0 ? O_CDENOSEEK : 0) + (EFI_UNSUPPORTED == pCdeAppIf->pIob[1].pFileProtocol->GetPosition(pCdeAppIf->pIob[1].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
                CDE_STDOUT->emufp = &pCdeAppIf->pIob[1];

                CDE_STDERR->pRootProtocol = pEfiShellParametersProtocol->StdErr;
                CDE_STDERR->pFileProtocol = pEfiShellParametersProtocol->StdErr;
                CDE_STDERR->fRsv = TRUE;
                CDE_STDERR->openmode = O_TEXT + O_WRONLY + O_CDESTDERR + (pCdeAppIf->STDOUT816BitMode == 0 ? O_CDENOSEEK : 0) + (EFI_UNSUPPORTED == pCdeAppIf->pIob[2].pFileProtocol->GetPosition(pCdeAppIf->pIob[2].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
                CDE_STDERR->emufp = &pCdeAppIf->pIob[2];
            }

            //
            // clock() initialization
            //
            pCdeAppIf->pCdeServices->TSClocksAtSystemStart = pCdeAppIf->pCdeServices->pGetTsc(pCdeAppIf);
            if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
                pCdeAppIf->pCdeServices->TSClocksPerSec = pCdeAppIf->pCdeServices->pGetTscPerSec(pCdeAppIf);
                pCdeAppIf->pCdeServices->TimeAtSystemStart = pCdeAppIf->pCdeServices->pGetTime(pCdeAppIf);
            }
            //todo check STATUS
            //Status = SystemTable->BootServices->LocateProtocol(&_gCdeDxeProtocolGuid,NULL,&pCdeAppIf->pCdeServices);

            pwcsCmdLine = pLoadedImageProtocol->LoadOptions;

            _cdeCoreSprintf(pCdeAppIf, (char*)pwcsCmdLine, "%S", (wchar_t*)pLoadedImageProtocol->LoadOptions);

            //
            // initialize argv[0] .. argv[CDE_ARGV_MAX] to pointer to ""
            //
            i = 0;
            do {
                szCmdLine[i] = (char)pwcsCmdLine[i];
            } while (szCmdLine[i++]);

            argc = _cdeStr2Argcv((char**)&pCdeAppIf->pArgvEx[2], (char*)pwcsCmdLine);

            //TODO: gszCdeDriverName = argvex[0 + 2];

            memset((void*)&pCdeAppIf->rgcbAtexit[0], (int)0, sizeof(pCdeAppIf->rgcbAtexit));

//TODO            if (1)
//TODO            {
//TODO                //
//TODO                // initialize C/C++ runtime library
//TODO                //
//TODO                    //if (_initterm_e(__xi_a, __xi_z) != 0){
//TODO                    //    Status = EFI_LOAD_ERROR;
//TODO                    //    break;
//TODO                    //}
//TODO                    //_initterm(__xc_a, __xc_z);
//TODO                typedef  void (*FUNCPTR)();
//TODO                typedef  int (*FUNCPTR2)();
//TODO
//TODO                /* inits */
//TODO                char* pFctEntry = (char*)&__xc_a;
//TODO                char* pFctEntry2 = (char*)&__xi_a;
//TODO                int nRet = 0;
//TODO                /*
//TODO                 * walk the table of function pointers from the bottom up, until
//TODO                 * the end is encountered.  Do not skip the first entry.  The initial
//TODO                 * value of pfbegin points to the first valid entry.  Do not try to
//TODO                 * execute what pfend points to.  Only entries before pfend are valid.
//TODO                 */
//TODO
//TODO                 /* walk through function pointer table */
//TODO                while ((void*)pFctEntry != (void*)&__xc_z)
//TODO                {
//TODO                    FUNCPTR pFct = *((FUNCPTR*)pFctEntry);
//TODO
//TODO                    if (pFct) {
//TODO                        //printf("<-->%p\n", pFct);
//TODO                        //getchar();
//TODO                        //__debugbreak();
//TODO                        pFct();
//TODO                    }
//TODO
//TODO                    pFctEntry = pFctEntry + sizeof(void*);
//TODO                }
//TODO
//TODO                /* walk through function pointer table */
//TODO                while (0 == nRet && ((void*)pFctEntry2 != (void*)&__xi_z))
//TODO                {
//TODO                    FUNCPTR2 pFct2 = *((FUNCPTR2*)pFctEntry2);
//TODO                    if (pFct2) {
//TODO                        nRet = pFct2();
//TODO                    }
//TODO                    pFctEntry2 = pFctEntry2 + sizeof(void*);
//TODO                    if (0 != nRet)
//TODO                        break;
//TODO                }
//TODO            }

            CDEPOSTCODE(IS64BITCODE, 0xC8);

            if (0 == __cdeGetCurrentPrivilegeLevel())        // running in RING0
                _enable();

            Status = setjmp(pCdeAppIf->exit_buf) ? pCdeAppIf->exit_status : main(argc, (char**)&pCdeAppIf->pArgvEx[2]);

            if (0 == __cdeGetCurrentPrivilegeLevel())        // running in RING0
                if (0 == (0x200 & eflags))          // restore IF interrupt flag
                    _disable();

            CDEPOSTCODE(IS64BITCODE, 0xC9);

            for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--) {
                if (NULL != pCdeAppIf->rgcbAtexit[i]) {
                    (*pCdeAppIf->rgcbAtexit[i])();
                }
            }

            //
            // close open files
            //
            for (i = 3/*skip stdin,stdout,stderr*/; i < CDE_FILEV_MAX; i++)
                fclose((FILE*)&pCdeAppIf->pIob[i]);

            //
            // flush and close stdout + stderr, if redirected
            //
            fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);    // NULL,EOF,0,stream == flush parameter
            fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDERR);    // NULL,EOF,0,stream == flush parameter

            for (i = 1/*skip stdin*/; i <= 2; i++)
                if (O_CDEREOPEN & pCdeAppIf->pIob[i].openmode)
                    fclose((FILE*)&pCdeAppIf->pIob[i]);

            //
            // free memory allocated during runtime
            //
            if (CDE_FREE_MEMORY_ALLOCATION_ON_EXIT) {
                HEAPDESC* pHeap = &pCdeAppIf->pCdeServices->HeapStart;
                do {
                    if (pHeap->qwMagic == ALLOCMEM) {
                        free(&pHeap[1]);
                        pHeap = &pCdeAppIf->pCdeServices->HeapStart;
                    }
                    pHeap = pHeap->pSucc;
                } while (pHeap);
            }//if(1)

            if ((0 == (O_CDEREDIR & CDE_STDOUT->openmode))
                && (0 == (O_CDEREDIR & CDE_STDERR->openmode)))
            {
                pSystemTable->ConOut->OutputString(pSystemTable->ConOut, L"\r\n");
            }

        } while (0);

        CDEPOSTCODE(IS64BITCODE, 0xCF);
        return Status;
}

EFI_STATUS _osifCdeUefiShellAppCRT0Service(
    
    IN CDE_APP_IF *pCdeAppIf,
    
    //IN EFI_STATUS(*pMainEntryPoint)                 (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable),
    
    IN void (*pProcessLibraryConstructorList)       (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable),
    IN EFI_STATUS (*pProcessModuleEntryPointList)   (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable),
    IN void (*pProcessLibraryDestructorList)        (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
)
{

    if (NULL != pProcessModuleEntryPointList)
    {
        // process EDK2 related library ctor/dtor list and entry point list 
    }

    _MainEntryPoint(pCdeAppIf);

    return EFI_SUCCESS;
}
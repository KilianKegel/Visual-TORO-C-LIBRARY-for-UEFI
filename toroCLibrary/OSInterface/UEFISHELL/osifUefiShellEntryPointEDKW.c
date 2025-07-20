/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiShellEntryPointEDK.c

Abstract:

    CRT0 for UEFI Shell in the EDK build environment

Author:

    Kilian Kegel

--*/
#define OS_EFI
//
// stdio.h
//
#define EOF    (-1)
//
// signal.h
//
#define SIGINT  2
//
// setjmp.h
//

#include <errno.h>
#include <limits.h>

#undef NULL
#include <cde.h>
#include <CdeServices.h>

#include <Library/DebugLib.h>
#include <Protocol\LoadedImage.h>
#include <Protocol\ShellParameters.h>
#include "Pi\PiStatusCode.h"
#include "Protocol\StatusCode.h"
#include <Protocol\Shell.h>
//
// setjmp.h
//
extern __declspec(dllimport) int setjmp(jmp_buf);

//
// C++/"C with classes" for the Microsoft compiler
//
    typedef int(__cdecl* _PIFV)(void);
    typedef void(__cdecl* _PVFV)(void);

    #pragma section(".CRT$XCA",read)
    #pragma section(".CRT$XCZ",read)
    #pragma section(".CRT$XIA",read)
    #pragma section(".CRT$XIZ",read)
    #pragma section(".CRT$XPA",read)
    #pragma section(".CRT$XPZ",read)
    #pragma section(".CRT$XTA",read)
    #pragma section(".CRT$XTZ",read)

    #define _CRTALLOC(x) __declspec(allocate(x))

    _CRTALLOC(".CRT$XIA") _PIFV* __xi_a;//[] = { (void*)0 }; // C initializers (first)
    _CRTALLOC(".CRT$XIZ") _PIFV* __xi_z;//[] = { (void*)0 }; // C initializers (last)
    _CRTALLOC(".CRT$XCA") _PVFV* __xc_a;//[] = { (void*)0 }; // C++ initializers (first)
    _CRTALLOC(".CRT$XCZ") _PVFV* __xc_z;//[] = { (void*)0 }; // C++ initializers (last)
    _CRTALLOC(".CRT$XPA") _PVFV* __xp_a;//[] = { (void*)0 }; // C pre-terminators (first)
    _CRTALLOC(".CRT$XPZ") _PVFV* __xp_z;//[] = { (void*)0 }; // C pre-terminators (last)
    _CRTALLOC(".CRT$XTA") _PVFV* __xt_a;//[] = { (void*)0 }; // C terminators (first)
    _CRTALLOC(".CRT$XTZ") _PVFV* __xt_z;//[] = { (void*)0 }; // C terminators (last)

    #pragma comment(linker, "/merge:.CRT=.rdata")

//
// externs
//
extern unsigned char _gCdeCfgCmdLnParmPoolMemRetain;
extern unsigned char _gCdeCfgCmdLnParmReportStatusCodeSTDOUT;

// CdeServices
extern _PUTCHAR         _CdeDbgPutChar;
extern VWXPRINTF        _cdeCoreVwxPrintf;
extern VWXSCANF         _cdeCoreVwxScanf;
extern WCSSTRTOK        _cdeCoreWcsStrTok;
extern WCSSTRPBRKSPN    _cdeWcsStrPbrkSpn;
extern void EFIAPI      DebugPrint(IN UINTN ErrorLevel, IN const char* Format, ...);
extern MEMREALLOC       _cdeCoreMemRealloc;
extern MEMSTRXCPY       _cdeMemStrxCpy;
extern MEMSTRXCMP       _cdeMemStrxCmp;
extern OSIFGETTIME      _osifIbmAtGetTime;
extern OSIFSETTIME      _osifIbmAtSetTime;
extern OSIFGETTSCPERSEC _osifUefiShellGetTscPerSec;
extern OSIFGETTSC       _osifIbmAtGetTsc;
extern OSIFMEMALLOC     _osifUefiShellMemAlloc;          /*pMemAlloc     */
extern OSIFMEMFREE      _osifUefiShellMemFree;           /*pfREEPages    */
extern OSIFFWRITE       _osifUefiShellFileWrite;         /*pFwrite       */
extern OSIFFREAD        _osifUefiShellFileRead;          /*pFread        */
extern OSIFFSETPOS      _osifUefiShellFileSetPos;        /*pFsetpos      */
extern OSIFFOPEN        _osifUefiShellFileOpen;          /*pFopen        */
extern OSIFFCLOSE       _osifUefiShellFileClose;         /*pFclose       */
extern OSIFFDELETE      _osifUefiShellFileDelete;        /*pFdelete      */
extern OSIFFRENAME      _osifUefiShellFileRename;        /*pFrename;.    */
extern OSIFFFINDALL     _osifUefiShellFileFindAll;       /*pFfindall     */
extern OSIFFGETSTATUS   _osifUefiShellFileGetStatus;     /*pFgetstatus   */
extern OSIFDIRCREATE    _osifUefiShellDirectoryCreate;   /*pDirCreate    */
extern OSIFCMDEXEC      _osifUefiShellCmdExec;           /*pCmdExec      */
extern OSIFGETENV       _osifUefiShellGetEnv;            /*pGetEnv       */
extern OSIFGETDCWD      _osifUefiShellGetDrvCwd;         /*pGetDrvCwd    current working directory*/

extern COREFILERW       _cdeCoreFread;                   /*pFReadCORE    */
extern COREFILERW       _cdeCoreFwrite;                  /*pFWriteCORE   */
extern CORESETPOS       _cdeCoreFsetpos;                 /*pFSetPosCORE  */
extern COREFFLUSH       _cdeCoreFflush;                  /*pFFlushCORE   */

// Microsoft specific C++ initialization support
typedef int(__cdecl* _PIFV)(void);
typedef void(__cdecl* _PVFV)(void);

extern _PIFV* __xi_a; // C initializers (first)
extern _PIFV* __xi_z; // C initializers (last)
extern _PVFV* __xc_a; // C++ initializers (first)
extern _PVFV* __xc_z; // C++ initializers (last)
extern _PVFV* __xp_a; // C pre-terminators (first)
extern _PVFV* __xp_z; // C pre-terminators (last)
extern _PVFV* __xt_a; // C terminators (first)
extern _PVFV* __xt_z; // C terminators (last)
extern int _initterm_e(_PIFV* pfbegin, _PIFV* pfend);
extern void _initterm(_PVFV* pfbegin, _PVFV* pfend);

extern int wmain(int argc, wchar_t** argv);
extern EFI_STATUS EFIAPI OemHookStatusCodeInitialize(void);
extern void* _CdeLocateProtocol(IN EFI_GUID* Protocol, IN void* Registration OPTIONAL/*,OUT void **Interface*/);

extern int _cdeStr2Argcv(char** argv, char* szCmdline);
extern void _cdeSigDflt(int sig);
extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;
extern struct lconv _cdeLconv_C;
extern char __cdeGetCurrentPrivilegeLevel(void);
extern EFI_GUID _gCdeDxeProtocolGuid;
extern EFI_GUID _gCdeEfiShellProtocolGuid;

extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) int fclose(FILE* stream);
extern __declspec(dllimport) int fgetc(FILE* stream);
extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
extern __declspec(dllimport) int sprintf(char* pszDest, const char* pszFormat, ...);
extern __declspec(dllimport) int raise(int sig);
extern __declspec(dllimport) size_t mbstowcs(wchar_t* pwcs, const char* s, size_t n);

//#undef setjmp
//extern __declspec(dllimport) int setjmp(jmp_buf);

//
// globals
//
EFI_HANDLE              _cdegImageHandle;
EFI_SYSTEM_TABLE*       _cdegST;
EFI_BOOT_SERVICES*      _cdegBS;
EFI_RUNTIME_SERVICES*   _cdegRT;
char* gszCdeDriverName;
static CDESYSTEMVOLUMES gCdeSystemVolumes = { .nVolumeCount = (UINTN)-1 };
static CDEFILE _iob[CDE_FILEV_MAX];                                  /* Microsoft definition. It must be buildable within the DDK*/
static EFI_GUID _gEfiStatusCodeRuntimeProtocolGuid = { 0xD2B2B828, 0x0826, 0x48A7, { 0xB3, 0xDF, 0x98, 0x3C, 0x00, 0x60, 0x24, 0xF0 } };
//
// include the original tianocore/UEFI DriverEntryPoint.c
//
#include "..\EDK2ObjBlocker\_cdeStdCIntrinsics_c.h"
#include "..\EDK2ObjBlocker\ApplicationEntryPoint_c.h"
#include "..\EDK2ObjBlocker\DebugLib_c.h"

#ifndef _DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_
/** _cdeCRT0UefiShellEDK()

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiShellEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
Description

    Function locates the "Application Interface" (r/w memory) that is exclusive for each driver
    at runtime.
    For PeiDrivers this chunk of memory is placed in a HOB, allocated at image startup.

    NOTE:   _ModuleEntryPoint() might run various LibConstructors that use DEBUG macro that CdePkg replaces.
            The CdeAppIf (CDE Application interface) needs to be allocated and initialized earlier, at
            image start.

    Returns

    @param[in]  void

    @retval EFI_STATUS

**/
EFI_STATUS EFIAPI _cdeCRT0UefiShellEDKW(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    return _ModuleEntryPoint(ImageHandle, SystemTable);
}

EFI_STATUS EFIAPI _cdeCRT0UefiShellEDKWINT3(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    __debugbreak();
    return _ModuleEntryPoint(ImageHandle, SystemTable);
}
#endif//_DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_



static void _StdOutPutChar(int c, void** ppDest) {
    if (c == EOF)
        fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);
    else
        fwrite(&c, 1, 1, (FILE*)CDE_STDOUT);
}

static int _StdInGetChar(void** ppDest) {
    return fgetc((FILE*)CDE_STDIN);
}

static CDE_SERVICES gCdeServicesShell = {/*CDE_PROTOCOL*/
    .TimeAtSystemStart = 0,         /* assuming 01.01.1970  */
    .TSClocksPerSec = 2000000000,   /* assuming 2GHz        */

    .wVerMajor = 0,
    .wVerMinor = 0,
    //////todo filedate??? __TIMESTAMP__
    ////    UINT16 wPtclSize;
    .fx64Opcode = 8 == sizeof(void*),
    .HeapStart = {(void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1},
    .TSClocksAtCdeTrace = 0,
    .TimeAtSystemStart = 0,
    .ReportStatusCode = { 0 },
    .pvEfiShellProtocol = 0,
    .pCdeSystemVolumes = &gCdeSystemVolumes,
    ////    FNDECL_MAINSTART(*pmainstart);          // the fjMainDxeEntryPoint/fjMainSmmEntryPoint loader     /* kg0705F0*/
    ////    FNDECL_SMMSTART(*psmmstart);            // wrapper for fjLoadDriverToSmm
    .pGetConIn = _StdInGetChar,
    .pPutConOut = _StdOutPutChar,
    .pPutDebug = _CdeDbgPutChar,
    .pVwxPrintf = _cdeCoreVwxPrintf,
    .pVwxScanf = _cdeCoreVwxScanf,
    .pMemRealloc = _cdeCoreMemRealloc,
    .pMemStrxCpy = _cdeMemStrxCpy,
    .pMemStrxCmp = _cdeMemStrxCmp,
    ////// ----- string processing functions
    .pWcsStrPbrkSpn = _cdeWcsStrPbrkSpn,
    .pWcsStrTok = _cdeCoreWcsStrTok,
//
// core C functions, running on driver side
//
    .pFReadCORE = _cdeCoreFread,                   /* core fread()    */
    .pFWriteCORE = _cdeCoreFwrite,                 /* core fwrite()   */
    .pFSetPosCORE = _cdeCoreFsetpos,               /* core fsetpos()  */
    .pFFlushCORE = _cdeCoreFflush,                 /* core fflush()   */

    // OSIF - operating system interface
    //
        .pGetTime = _osifIbmAtGetTime,
        .pSetRtcTime = 0,
        .pGetTscPerSec = _osifUefiShellGetTscPerSec,
        .pGetTsc = _osifIbmAtGetTsc,
        .pMemAlloc = _osifUefiShellMemAlloc,
        .pMemFree = _osifUefiShellMemFree,
        .pFopen = _osifUefiShellFileOpen,            /* OSIFFOPEN        *pFopen     */
        .pFclose = _osifUefiShellFileClose,          /* OSIFFCLOSE       *pFclose    */
        .pFread = _osifUefiShellFileRead,            /* OSIFFREAD        *pFread     */
        .pFwrite = _osifUefiShellFileWrite,          /* OSIFFWRITE       *pFwrite    */
        .pFsetpos = _osifUefiShellFileSetPos,        /* OSIFFSETPOS      *pFsetpos   */
        .pFdelete = _osifUefiShellFileDelete,        /* OSIFFDELETE      *pFdelete   */
        .pFrename = _osifUefiShellFileRename,        /* OSIFFRENAME      *pFrename   */
        .pCmdExec = _osifUefiShellCmdExec,           /* OSIFCMDEXEC      *pCmdExec   */
        .pGetEnv = _osifUefiShellGetEnv,             /* OSIFGETENV       *pGetEnv    */
        .pFfindall = _osifUefiShellFileFindAll,      /* OSIFFFINDALL     *pFfindall  */
        .pFgetstatus = _osifUefiShellFileGetStatus,  /* OSIFFGETSTATUS   *pFgetstatus*/
        .pGetDrvCwd = _osifUefiShellGetDrvCwd,       /* OSIFGETDCWD      *pGetDrvCwd */
        .pDirCreate = _osifUefiShellDirectoryCreate, /* OSIFDIRCREATE    *pDirCreate */
        .pDirRemove = NULL,                         /* OSIFDIRCREATE    *pDirCreate */
};

CDE_APP_IF CdeAppIfShellW = {

    .nErrno = 0,
    .nEfiErrno = 0,
    
    .pCdeServices = &gCdeServicesShell,
    {
        .BsDriverParm.OSIf = SHELLIF,
        .BsDriverParm.ImageHandle = 0,
        .BsDriverParm.pSystemTable = 0
    },
    .pStrtokStaticInternal = 0,
    .nNextRandStaticInternal = 0,
    .exit_buf = {0},
    .exit_status = EFI_SUCCESS,
    .rgcbAtexit = {0},
    .lTmpNamNum = 0,
    .rgfnSignal = {_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt},
    .pIob = &_iob[0],
    .cIob = CDE_FILEV_MAX,
    .bmRuntimeFlags = 0,    /* clear all runtimeflags   */
    .ActiveLocale = { "C", &_cdeLconv_C, NULL }/*&_cdeCLocale*/
};

static void __CdeChkCtrlC(void) {

    EFI_SHELL_PROTOCOL* pEfiShellProtocol = CdeAppIfShellW.pCdeServices->pvEfiShellProtocol;

    if(NULL != pEfiShellProtocol)
        if (_cdegBS->CheckEvent(pEfiShellProtocol->ExecutionBreak) == EFI_SUCCESS)
            raise(SIGINT);
}

void* __cdeGetAppIf(void) {
    __CdeChkCtrlC();
    return &CdeAppIfShellW;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
_MainEntryPointShellW(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE* SystemTable
)
{
    void* argvex[CDE_ARGV_MAX + 2];         // DON'T INITIALIZE HERE, BECAUSE C COMPILER inserts memset()!!!
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
        /*fUEFIFormat           */ 0 
    };
    size_t eflags = __cdeGetEFLAGS();
    CDE_APP_IF* pCdeAppIf = &CdeAppIfShellW;

    CDEPOSTCODE(IS64BITCODE, 0xC0);
    if (1)
        do {


            _cdegImageHandle = ImageHandle;
            _cdegST = SystemTable;
            _cdegBS = SystemTable->BootServices;

            memset(argvex, 0, sizeof(argvex));      // instead do dedicated initialization that invokes __imp__memset()
            argvex[0] = (void*)ImageHandle;
            argvex[1] = (void*)SystemTable;//ADD SUPPORT FOR argv[-1] argv[-2]

            pCdeAppIf->DriverParm.BsDriverParm.ImageHandle = ImageHandle;
            pCdeAppIf->DriverParm.BsDriverParm.pSystemTable = SystemTable;

            //
            // get the ReportStatusCodeProtocol
            //
            Status = _cdegBS->LocateProtocol(&_gEfiStatusCodeRuntimeProtocolGuid, NULL, (void**)&gCdeServicesShell.ReportStatusCode.pShell);

            if (EFI_SUCCESS != Status) { //todo: add error handler
                if ((void*)0 == SystemTable->ConOut || (void*)-1 == SystemTable->ConOut) {
                    CDEPOSTCODE(IS64BITCODE, 0xC4);
                    CDEDEADLOOP(IS64BITCODE, 0xC4);
                    CDEDEBUGBREAK(!IS64BITCODE, 0xC4);
                }
                gCdeServicesShell.ReportStatusCode.pShell = NULL;
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"FATAL ERROR: Report Status Code Protocol missing\r\n");
                Status = EFI_PROTOCOL_ERROR;
                break;
            }

            ////
            //// check shell version
            ////
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
                pCdeAppIf->pCdeServices->pvEfiShellProtocol = _CdeLocateProtocol(&_gCdeEfiShellProtocolGuid, NULL);

                if (NULL != pCdeAppIf->pCdeServices->pvEfiShellProtocol)
                {
                    EFI_SHELL_PROTOCOL *pEfiShellProtocol = pCdeAppIf->pCdeServices->pvEfiShellProtocol;
                    ShellVerNum = 100 * pEfiShellProtocol->MajorVersion;
                    ShellVerNum += pEfiShellProtocol->MinorVersion;
                }

                if (202 > ShellVerNum) {
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"FATAL ERROR: Shell Protocol missing or below v2.2\r\nDownload UEFI SHELL from:\r\n    https://github.com/tianocore/edk2/blob/edk2-stable201903/ShellBinPkg/UefiShell/X64/Shell.efi\r\nand copy to EFI\\BOOT\\BOOTX64.EFI\r\n");
                    Status = EFI_INCOMPATIBLE_VERSION;
                    break;
                }
            }

            Status = SystemTable->BootServices->HandleProtocol(ImageHandle, &EfiLoadedImageProtocolGuid, (void**)&pLoadedImageProtocol);

            if (1/* setup file i/o, stdin, stout, stderr*/) {
                UINT64 Position;
                static EFI_GUID EfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;
                EFI_SHELL_PARAMETERS_PROTOCOL* pEfiShellParametersProtocol;

                memset(pCdeAppIf->pCdeServices->pCdeSystemVolumes, 0, sizeof(CDESYSTEMVOLUMES));   // clear entire structure
                pCdeAppIf->pCdeServices->pCdeSystemVolumes->nVolumeCount = (UINTN) -1;             // mark volume enumeration not yet done

                memset(&_iob[0], 0, sizeof(_iob));                          // clear entire structure

                Status = SystemTable->BootServices->HandleProtocol(ImageHandle, &EfiShellParametersProtocolGuid, (void**)&pEfiShellParametersProtocol);

                CDE_STDIN->pRootProtocol = pEfiShellParametersProtocol->StdIn;

                CDE_STDIN->pFileProtocol = pEfiShellParametersProtocol->StdIn;
                CDE_STDIN->fRsv = TRUE;
                CDE_STDIN->openmode = O_TEXT + O_RDONLY + O_CDESTDIN + O_CDENOSEEK + (EFI_UNSUPPORTED == _iob[0].pFileProtocol->GetPosition(_iob[0].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
                CDE_STDIN->emufp = &_iob[0];

                CDE_STDOUT->pRootProtocol = pEfiShellParametersProtocol->StdOut;
                CDE_STDOUT->pFileProtocol = pEfiShellParametersProtocol->StdOut;
                CDE_STDOUT->fRsv = TRUE;
                CDE_STDOUT->openmode = O_TEXT + O_WRONLY + O_CDESTDOUT + (pCdeAppIf->STDOUT816BitMode == 0 ? O_CDENOSEEK : 0) + (EFI_UNSUPPORTED == _iob[1].pFileProtocol->GetPosition(_iob[1].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
                CDE_STDOUT->emufp = &_iob[1];

                CDE_STDERR->pRootProtocol = pEfiShellParametersProtocol->StdErr;
                CDE_STDERR->pFileProtocol = pEfiShellParametersProtocol->StdErr;
                CDE_STDERR->fRsv = TRUE;
                CDE_STDERR->openmode = O_TEXT + O_WRONLY + O_CDESTDERR + (pCdeAppIf->STDOUT816BitMode == 0 ? O_CDENOSEEK : 0) + (EFI_UNSUPPORTED == _iob[2].pFileProtocol->GetPosition(_iob[2].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
                CDE_STDERR->emufp = &_iob[2];
            }

            //
            // clock() initialization
            //
            pCdeAppIf->pCdeServices->TSClocksAtSystemStart = gCdeServicesShell.pGetTsc(pCdeAppIf);
            if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
                pCdeAppIf->pCdeServices->TSClocksPerSec = gCdeServicesShell.pGetTscPerSec(pCdeAppIf,0);
                pCdeAppIf->pCdeServices->TimeAtSystemStart = gCdeServicesShell.pGetTime(pCdeAppIf);
            }
            //todo check STATUS
            //Status = SystemTable->BootServices->LocateProtocol(&_gCdeDxeProtocolGuid,NULL,&CdeAppIfShellW.pCdeServices);

            pwcsCmdLine = pLoadedImageProtocol->LoadOptions;

            sprintf((char*)pwcsCmdLine, "%S", (wchar_t*)pLoadedImageProtocol->LoadOptions);

            //
            // initialize argv[0] .. argv[CDE_ARGV_MAX] to pointer to ""
            //
            i = 0;
            do {
                szCmdLine[i] = (char)pwcsCmdLine[i];
            } while (szCmdLine[i++]);
            
            argc = _cdeStr2Argcv((char**)&argvex[0 + 2], (char*)pwcsCmdLine);

            gszCdeDriverName = argvex[0 + 2];

            memset((void*)pCdeAppIf->rgcbAtexit[0], (int)0, sizeof(pCdeAppIf->rgcbAtexit));
            
            if (1)
            {
                //
                // initialize C/C++ runtime library
                //
                    //if (_initterm_e(__xi_a, __xi_z) != 0){
                    //    Status = EFI_LOAD_ERROR;
                    //    break;
                    //}
                    //_initterm(__xc_a, __xc_z);
                typedef  void (*FUNCPTR)();
                typedef  int (*FUNCPTR2)();

                /* inits */
                char* pFctEntry = (char*)&__xc_a;
                char* pFctEntry2 = (char*)&__xi_a;
                int nRet = 0;
                /*
                 * walk the table of function pointers from the bottom up, until
                 * the end is encountered.  Do not skip the first entry.  The initial
                 * value of pfbegin points to the first valid entry.  Do not try to
                 * execute what pfend points to.  Only entries before pfend are valid.
                 */

                 /* walk through function pointer table */
                while ((void*)pFctEntry != (void*)&__xc_z)
                {
                    FUNCPTR pFct = *((FUNCPTR*)pFctEntry);

                    if (pFct) {
                        //printf("<-->%p\n", pFct);
                        //getchar();
                        //__debugbreak();
                        pFct();
                    }

                    pFctEntry = pFctEntry + sizeof(void*);
                }

                /* walk through function pointer table */
                while (0 == nRet && ((void*)pFctEntry2 != (void*)&__xi_z))
                {
                    FUNCPTR2 pFct2 = *((FUNCPTR2*)pFctEntry2);
                    if (pFct2) {
                        nRet = pFct2();
                    }
                    pFctEntry2 = pFctEntry2 + sizeof(void*);
                    if (0 != nRet)
                        break;
                }
            }

            CDEPOSTCODE(IS64BITCODE, 0xC8);

            if (0 == __cdeGetCurrentPrivilegeLevel())        // running in RING0
                __CDEINTERRUPT_ENABLE;

            //
            // conform all narrow argv to wide strings
            //
            for (i = 0; i < argc; i++)                                      // convert all argv to wchar_t*
            {                                                               //
                size_t strsize = sizeof((char)'\0') + strlen(argvex[2 + i]);// argv[i] narrow string size
                wchar_t* pwcs = malloc(sizeof(wchar_t) * strsize);          // allocate buffer for corresponding wide string
                                                                            //
                mbstowcs(pwcs, argvex[2 + i], INT_MAX);                     // convert str to wcs
                argvex[2 + i] = (void*)pwcs;                                // overwrite argv pointer
            }
            Status = setjmp(pCdeAppIf->exit_buf) ? pCdeAppIf->exit_status : wmain(argc, (wchar_t**) & argvex[0 + 2]);

            if (0 == __cdeGetCurrentPrivilegeLevel())        // running in RING0
                if (0 == (0x200 & eflags))          // restore IF interrupt flag
                    __CDEINTERRUPT_DISABLE;

            CDEPOSTCODE(IS64BITCODE, 0xC9);

            for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--) {
                if (NULL != pCdeAppIf->rgcbAtexit[i]) {
                    (pCdeAppIf->rgcbAtexit[i])();
                }
            }

            //
            // close open files
            //
            for (i = 3/*skip stdin,stdout,stderr*/; i < CDE_FILEV_MAX; i++)
                fclose((FILE*)&_iob[i]);

    //
    // close open files
    //
            fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);    // NULL,EOF,0,stream == flush parameter
            fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDERR);    // NULL,EOF,0,stream == flush parameter

            for (i = 1/*skip stdin*/; i <= 2; i++)
                if (O_CDEREOPEN & _iob[i].openmode)
                    fclose((FILE*)&_iob[i]);

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
            
            if (    (0 == (O_CDEREDIR & CDE_STDOUT->openmode))
                &&  (0 == (O_CDEREDIR & CDE_STDERR->openmode)))
            {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
            }

        } while (0);

        CDEPOSTCODE(IS64BITCODE, 0xCF);
        return Status;
}
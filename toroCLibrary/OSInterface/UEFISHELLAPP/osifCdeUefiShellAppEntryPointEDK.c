/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifCdeUefiShellAppEntryPointEDK.c

Abstract:
    ###################################################################################
    ### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
    ###################################################################################

    CRT0 for UEFI DXE in the EDK build environment

Author:

    Kilian Kegel

--*/
#define OS_EFI
#undef NCDETRACE
//
// stdio.h
//
#define EOF    (-1)

#define ASSERT
#include <uefi.h>
#include <cde.h>
//#undef NULL
#include <CdeServices.h>
#include <Protocol/smmbase2.h>

//
// prototypes
//
static void __cdeFatalCdeServicesNotAvailCdeUefiShellDrv(EFI_SYSTEM_TABLE* SystemTable);
static int _StdInGetCharDXE(void** ppDest);
static int _StdInGetCharSHELL(void** ppDest);
static void _StdOutPutChar(int c, void** ppDest);

EFI_STATUS main(int argc, char** argv);

//
// externals
//
EFI_HANDLE               _cdegImageHandle;
EFI_SYSTEM_TABLE*        _cdegST;
EFI_BOOT_SERVICES*       _cdegBS;
EFI_RUNTIME_SERVICES*    _cdegRT;

extern EFI_HANDLE           gImageHandle;
extern EFI_SYSTEM_TABLE*    gST;
extern EFI_BOOT_SERVICES*   gBS;
extern EFI_RUNTIME_SERVICES*gRT;

//extern void _cdeSigDflt(int sig);
//extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;
//extern struct lconv _cdeLconv_C;
//extern GUID gEfiCallerIdGuid;
//extern int _cdeStr2Argcv(char** argv, char* szCmdline);
//extern char __cdeGetCurrentPrivilegeLevel(void);

//extern EFI_GUID gEfiLoadedImageProtocolGuid;
//extern EFI_GUID _gCdeDxeProtocolGuid;                   // The GUID for the protocol
//extern EFI_GUID _gCdeShellProtocolGuid;                 // The GUID for the protocol

extern void _disable(void);
extern void _enable(void);

//extern _PUTCHAR  _CdeDbgPutChar;
//extern VWXPRINTF _cdeVwxPrintf;
//extern VWXSCANF _cdeVwxScanf;
//extern WCSSTRTOK _cdeWcsStrTok;
//extern WCSSTRPBRKSPN _cdeWcsStrPbrkSpn;
//extern VOID EFIAPI DebugPrint(IN UINTN ErrorLevel, IN CONST CHAR8* Format, ...);
//extern MEMREALLOC       _cdeMemRealloc;
//extern MEMSTRXCPY       _cdeMemStrxCpy;
//extern MEMSTRXCMP       _cdeMemStrxCmp;
//extern OSIFGETTIME		_osifIbmAtGetTime;
//extern OSIFSETTIME      _osifIbmAtSetTime;
//extern OSIFGETTSCPERSEC _osifIbmAtGetTscPerSec;
//extern OSIFGETTSC       _osifIbmAtGetTsc;
//extern OSIFMEMALLOC     _osifUefiShellMemAlloc;          /*pMemAlloc     */
//extern OSIFMEMFREE      _osifUefiShellMemFree;           /*pfREEPages    */
//extern OSIFFWRITE       _osifUefiShellFileWrite;         /*pFwrite       */
//extern OSIFFREAD        _osifUefiShellFileRead;          /*pFread        */
//extern OSIFFSETPOS      _osifUefiShellFileSetPos;        /*pFsetpos      */
//extern OSIFFOPEN        _osifUefiShellFileOpen;          /*pFopen        */
//extern OSIFFCLOSE       _osifUefiShellFileClose;         /*pFclose       */
//extern OSIFFDELETE      _osifUefiShellFileDelete;        /*pFdelete      */
//extern OSIFFRENAME      _osifUefiShellFileRename;        /*pFrename;.    */
//extern OSIFFFINDALL     _osifUefiShellFileFindAll;       /*pFfindall     */
//extern OSIFFGETSTATUS   _osifUefiShellFileGetStatus;     /*pFgetstatus   */
//extern OSIFDIRCREATE    _osifUefiShellDirectoryCreate;   /*pDirCreate    */
//extern OSIFCMDEXEC      _osifUefiShellCmdExec;           /*pCmdExec      */
//extern OSIFGETENV       _osifUefiShellGetEnv;            /*pGetEnv       */
//extern OSIFGETDCWD      _osifUefiShellGetDrvCwd;         /*pGetDrvCwd    current working directory*/

//extern OSIFMEMALLOC   _osifUefiDxeMemAlloc/*pMemAlloc*/;
//extern OSIFMEMFREE    _osifUefiDxeMemFree/*pfREEPages*/;

#pragma intrinsic (_disable, _enable)

//extern __declspec(dllimport) void* malloc(size_t size);
//extern __declspec(dllimport) void free(void* ptr);
//extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
//extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
//extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
//extern __declspec(dllimport) int fgetc(FILE* stream);
//extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);

//
// UefiMain entry hook
//
extern EFI_STATUS EFIAPI UefiMainCDEHOOKED(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
extern EFI_GUID _gCdeEfiShellProtocolGuid;
extern EFI_EVENT EFIAPI EfiCreateProtocolNotifyEvent(
    IN  EFI_GUID* ProtocolGuid,
    IN  EFI_TPL           NotifyTpl,
    IN  EFI_EVENT_NOTIFY  NotifyFunction,
    IN  VOID* NotifyContext   OPTIONAL,
    OUT VOID** Registration
);

//
// globals
//
char* gszCdeDriverName;
static CDEFILE _iob[CDE_FILEV_MAX];                                  /* Microsoft definition. It must be buildable within the DDK*/

static CDE_APP_IF CdeAppIfCdeUefiShellApp;

/** __cdeGetAppIf()

Synopsis
    void* __cdeGetAppIf(void);
Description

    Function locates the "Application Interface" (r/w memory) that is exclusive for each driver
    at runtime.
    For PeiDrivers this chunk of memory is placed in a HOB, allocated at startup

    Returns

    @param[in]  void

    @retval (void*)pCdeAppIfDxe

**/
void* __cdeGetAppIf(void)
{
    return &CdeAppIfCdeUefiShellApp;
}

//static void _StdOutPutChar(int c, void** ppDest) {
//    if (c == EOF)
//        fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);
//    else
//        fwrite(&c, 1, 1, (FILE*)CDE_STDOUT);
//}
//
//static int _StdInGetCharSHELL(void** ppDest) {
//    return fgetc((FILE*)CDE_STDIN);
//}
//
//static int _StdInGetCharDXE(void** ppDest) {
//    return EOF;
//}
//

//
// include the original tianocore/UEFI DriverEntryPoint.c
//  NOTE:   INCLUDEing the sourcecode of PeimEntryPoint.c and DebugLib.c into _one_ _single_ .C file
//          forces the COMPILER to create _one_ _single_ .OBJ module that contains all symbols
//          needed create the proper .EFI image. 
//          Doing so, the LINKER won't search those symbols and find in other .OBJ that might block
//          and break function overloading of CdePkg.
//          
//          Function overloading is needed for the unique feature of driver-locale DEBUG Traces that
//          expose additionally the origin (driver, file, line, function) and the timestamp:
// 
//          * _ModuleEntryPoint() with removed ASSERT-invocation
//          * BaseDebugLibSerialPortConstructor()
//          * DebugPrint()
//          * DebugVPrint()
//          * DebugBPrint()
//          * DebugAssert()
//          * DebugClearMemory()
//          * DebugAssertEnabled()
//          * DebugPrintEnabled()
//          * DebugCodeEnabled()
//          * DebugClearMemoryEnabled()
//          * DebugPrintLevelEnabled()
#include "..\EDK2ObjBlocker\_cdeStdCIntrinsics_c.h"
#include "..\EDK2ObjBlocker\DriverEntryPoint_c.h"
#include "..\EDK2ObjBlocker\DebugLib_c.h"

#ifndef _DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

/** _cdeCRT0CdeUefiShellDrvEDK()

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0CdeUefiShellDrvEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
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
EFI_STATUS EFIAPI _cdeCRT0CdeUefiShellAppEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;

    //
    // initialize EDK global variables provided by UefiBootServicesTableLib.lib and UefiRuntimeServicesTableLib.lib
    //
    _cdegImageHandle = ImageHandle;
    _cdegST = SystemTable;
    _cdegBS = SystemTable->BootServices;
    _cdegRT = SystemTable->RuntimeServices;
    

    return EFI_SUCCESS == Status ? _ModuleEntryPoint(ImageHandle, SystemTable) : Status;    // run ProcessLibraryConstructorList(), 
                                                                                            // ProcessModuleEntryPointList(), provided 
                                                                                            // in AutoGen.c ...
}

EFI_STATUS EFIAPI _cdeCRT0CdeUefiShellAppEDKINT3(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    __debugbreak();
    return _cdeCRT0CdeUefiShellAppEDK(ImageHandle, SystemTable);
}
#endif//_DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

EFI_STATUS EFIAPI _MainEntryPointCdeUefiShellApp(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_LOAD_ERROR;
    static void* argvex[CDE_ARGV_MAX + 2];

    argvex[0] = (void*)ImageHandle;
    argvex[1] = (void*)SystemTable;//ADD SUPPORT FOR argv[-1] argv[-2]

    gImageHandle = _cdegImageHandle;
    gST = _cdegST;
    gBS = _cdegBS;
    gRT = _cdegRT;

    //
    // LoadOptions / command line interface
    //
    static EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
    size_t eflags = __readeflags();

    do {


    } while (0);

    return Status;
}

/**

Synopsis
    void __cdeFatalCdeServicesNotAvailDXE(void);
Description
    Function is invoked if CdeServices not found and writes error message via ReportStatusCode.

Returns

    @param[in]  PeiServices

    @retval

**/
static void __cdeFatalCdeServicesNotAvailCdeUefiShellApp(EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;
    static EFI_GUID EfiStatusCodeRuntimeProtocolGuid = { 0xD2B2B828, 0x0826, 0x48A7, { 0xB3, 0xDF, 0x98, 0x3C, 0x00, 0x60, 0x24, 0xF0 } };
    EFI_REPORT_STATUS_CODE* pReportStatucCode;

    typedef struct _DEBUGNFO {
        EFI_STATUS_CODE_DATA Header;
        unsigned  ErrorLevel;
        unsigned long long            parm[12];
        char             szText[sizeof("\r\n\r\nFATAL ERROR : CdeServices CDEUEFISHELLAPP not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n")];
    }DEBUGNFO;


    static DEBUGNFO DbgNfo = {
                    .Header.HeaderSize = sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Size = sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Type = { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
                    .ErrorLevel = 0xFF,
                    .parm = {0,0,0,0,0,0,0,0,0,0,0,0},
                    .szText = "\r\n\r\nFATAL ERROR : CdeServices CDEUEFISHELLAPP not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n"
    };

    //
    // locate the protocols needed to run CdeLib
    //
    Status = SystemTable->BootServices->LocateProtocol(&EfiStatusCodeRuntimeProtocolGuid, NULL, (void**)&pReportStatucCode);

    if(EFI_SUCCESS == Status)
        (*pReportStatucCode)(EFI_DEBUG_CODE, (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED), 0, NULL, (void*)&DbgNfo);
}

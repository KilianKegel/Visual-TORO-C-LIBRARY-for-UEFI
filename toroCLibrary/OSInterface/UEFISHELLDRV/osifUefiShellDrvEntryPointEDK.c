/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiShellDrvEntryPointEDK.c

Abstract:

    CRT0 for UEFI SHELL component in the EDK build environment

Author:

    Kilian Kegel

--*/
#define OS_EFI
//#undef NCDETRACE
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
#include <Protocol/loadedimage.h>

#include <Guid\Acpi.h>
#include <Protocol\AcpiSystemDescriptionTable.h>
#include <Protocol\AcpiTable.h>
#include <Guid\Acpi.h>
#include <IndustryStandard/Acpi62.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>

#include <Protocol\AcpiSystemDescriptionTable.h>
//
// setjmp.h
//
extern __declspec(dllimport) int setjmp(jmp_buf);

extern void _mysleep(int millisec);

//
// prototypes
//
static void __cdeFatalCdeServicesNotAvailUefiShellDrv(EFI_SYSTEM_TABLE* SystemTable);
static int _StdInGetCharDXE(void** ppDest);
static int _StdInGetCharSHELL(void** ppDest);
static void _StdOutPutChar(int c, void** ppDest);
static void notifyCB(EFI_EVENT Event, VOID* Context);
VOID* pShellProtocolRegistration;
static /*_PUTCHAR*/ void shellputc(int c, void** ppDest);
static /*_GETCHAR*/ int shellgetc(void** ppDest);

int main(int argc, char** argv);

//
// globals
//
EFI_HANDLE               _cdegImageHandle;
EFI_SYSTEM_TABLE*        _cdegST;
EFI_BOOT_SERVICES*       _cdegBS;
EFI_RUNTIME_SERVICES*    _cdegRT;

//
// externals
//
extern EFI_HANDLE           gImageHandle;
extern EFI_SYSTEM_TABLE*    gST;
extern EFI_BOOT_SERVICES*   gBS;
extern EFI_RUNTIME_SERVICES*gRT;
extern EFI_FILE_PROTOCOL  FileInterfaceStdIn;   // access directly in the UEFI SHELL binary
extern EFI_FILE_PROTOCOL  FileInterfaceStdOut;  // access directly in the UEFI SHELL binary
extern EFI_FILE_PROTOCOL  FileInterfaceStdErr;  // access directly in the UEFI SHELL binary

extern void _cdeSigDflt(int sig);
extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;
extern struct lconv _cdeLconv_C;
extern GUID gEfiCallerIdGuid;
extern int _cdeStr2Argcv(char** argv, char* szCmdline);
extern char __cdeGetCurrentPrivilegeLevel(void);

extern EFI_GUID gEfiLoadedImageProtocolGuid;
extern EFI_GUID _gCdeDxeProtocolGuid;                   // The GUID for the protocol
extern EFI_GUID _gCdeShellProtocolGuid;                 // The GUID for the protocol

extern EFI_GUID _gEfiStatusCodeRuntimeProtocolGuid;

extern CRT0SERVICE      _osifCdeUefiShellAppCRT0Service; 
extern _PUTCHAR         _CdeDbgPutChar;
extern VWXPRINTF        _cdeCoreVwxPrintf;
extern VWXSCANF         _cdeCoreVwxScanf;
extern WCSSTRTOK        _cdeCoreWcsStrTok;
extern WCSSTRPBRKSPN    _cdeWcsStrPbrkSpn;
extern VOID EFIAPI      DebugPrint(IN UINTN ErrorLevel, IN CONST CHAR8* Format, ...);
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

extern OSIFMEMALLOC   _osifUefiDxeMemAlloc/*pMemAlloc*/;
extern OSIFMEMFREE    _osifUefiDxeMemFree/*pfREEPages*/;

extern COREFILERW       _cdeCoreFread;              /*pFReadCORE    */
extern COREFILERW       _cdeCoreFwrite;             /*pFWriteCORE   */
extern CORESETPOS       _cdeCoreFsetpos;            /*pFSetPosCORE  */
extern COREFFLUSH       _cdeCoreFflush;             /*pFFlushCORE   */

extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
extern __declspec(dllimport) void* memcpy(void* s1, const void* s2, size_t n);
extern __declspec(dllimport) int memcmp(const void* pszDst, const void* pszSrc, size_t count);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
extern __declspec(dllimport) int fgetc(FILE* stream);
extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
extern __declspec(dllimport) int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n);
extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);
extern __declspec(dllimport) int strcmp(const char* pszDst, const char* pszSrc);
extern __declspec(dllimport) int fprintf(FILE* stream, const char* pszFormat, ...);
extern char _gCdeStrLibcVersion[];

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID))) //guiddef.h

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
extern unsigned _gCdeRTUefiShellInstanceType;
//NOTE: 0:  initial boot type, Shell is started during system boot, w/o command line (pLoadedImageProtocol->LoadOptions == L"")
//      1:  EFI_SHELL_PROTOCOL.Execute() type, Shell instance was invoked with the "Shell.efi -exit 'SHELL_COMMAND'",
//              For this case suppress F8-delay and supress all output messages 
//      2:  not yet defined

//
// globals
//
char* gszCdeDriverName;
static CDESYSTEMVOLUMES gCdeSystemVolumes = { .nVolumeCount = (UINTN) -1};
static CDEFILE _iob[CDE_FILEV_MAX];                                  /* Microsoft definition. It must be buildable within the DDK*/
unsigned _gCdeRTUefiShellInstanceType = 0;
//
// CdeServices with CdeDxeProtocol (CDE_DXE_PROTOCOL_GUID) for the CDE UEFI SHELL internal usage
//
static CDE_SERVICES gCdeServicesUefiShellDrv = {/*CDE_PROTOCOL*/
    .TimeAtSystemStart = 0,         /* assuming 01.01.1970  */
    .TSClocksPerSec = 2000000000,   /* assuming 2GHz        */

    .wVerMajor = 0,
    .wVerMinor = 0,
    .fx64Opcode = 8 == sizeof(void*),
    .fMemoryDiscovered = 1,         /* usually CdeServicesDxe starts AFTER EfiPeiMemoryDiscovered */
    .HeapStart = {(void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1},

    .pGetConIn = _StdInGetCharDXE,
    .pPutConOut = shellputc/*_StdOutPutChar*/,

    .pPutDebug = _CdeDbgPutChar,
    .pVwxPrintf = _cdeCoreVwxPrintf,
    .pVwxScanf = _cdeCoreVwxScanf,
    .pMemRealloc = _cdeCoreMemRealloc,

    .pMemStrxCpy = _cdeMemStrxCpy,
    .pMemStrxCmp = _cdeMemStrxCmp,
    .pWcsStrPbrkSpn = _cdeWcsStrPbrkSpn,
    .pWcsStrTok = _cdeCoreWcsStrTok,

    .pGetTime = _osifIbmAtGetTime,
    .pSetRtcTime = 0,
    .pGetTscPerSec = _osifUefiShellGetTscPerSec,
    .pGetTsc = _osifIbmAtGetTsc,
    .pMemAlloc = _osifUefiDxeMemAlloc,
    .pMemFree = _osifUefiDxeMemFree,

    .pFopen = 0,
    .pFclose = 0,
    .pFread = 0,
    .pFwrite = 0,
    .pFsetpos = 0,
    .pFdelete = 0,
    .pFrename = 0,
    .pCmdExec = 0,
    .pGetEnv = 0,
};

//
// CdeServices with CdeShellProtocol (CDE_SHELL_PROTOCOL_GUID) for CDE UEFI SHELL APPLICATION usage
//
static CDE_SERVICES gCdeServicesCdeUefiShellApp = {/*CDE_PROTOCOL*/

    .TimeAtSystemStart = 0,                 /* assuming 01.01.1970  */
    .TSClocksPerSec = 2000000000,           /* assuming 2GHz        */

    .wVerMajor = 0,
    .wVerMinor = 0,
    .fx64Opcode = 8 == sizeof(void*),
    .HeapStart = {(void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1},
    .TSClocksAtCdeTrace = 0,
    .TimeAtSystemStart = 0,
    .ReportStatusCode = { 0 },
    .pvEfiShellProtocol = 0,
    .pCRT0Service = NULL,//TODO _osifCdeUefiShellAppCRT0Service,
    .pCdeSystemVolumes = &gCdeSystemVolumes,

    .pGetConIn = shellgetc,
    .pPutConOut = shellputc/*_StdOutPutChar*/,
    .pPutDebug = _CdeDbgPutChar,
    .pVwxPrintf = _cdeCoreVwxPrintf,
    .pVwxScanf = _cdeCoreVwxScanf,
    .pMemRealloc = _cdeCoreMemRealloc,
    .pMemStrxCpy = _cdeMemStrxCpy,
    .pMemStrxCmp = _cdeMemStrxCmp,
//
// string processing functions
//
    .pWcsStrPbrkSpn = _cdeWcsStrPbrkSpn,
    .pWcsStrTok = _cdeCoreWcsStrTok,
//
// core C functions, running on driver side
//
    .pFReadCORE = _cdeCoreFread,            /* core fread()     */
    .pFWriteCORE = _cdeCoreFwrite,          /* core fwrite()    */
    .pFSetPosCORE = _cdeCoreFsetpos,        /* core fsetpos()   */
    .pFFlushCORE = _cdeCoreFflush,          /* core fflush()    */

//
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
    .pDirRemove = NULL,                          /* OSIFDIRCREATE    *pDirCreate */
};

static CDE_APP_IF CdeAppIfUefiShellDrv/* this is a DXE-type */ = {

    .nErrno = 0,
    .nEfiErrno = 0,

    .pCdeServices = &gCdeServicesUefiShellDrv,
    {
        .BsDriverParm.OSIf = DXEIF/* UefiShellDrv is a DXE Interface */,
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
    .cIob = 0,      //CDE_FILEV_MIN,  /* _iobCnt,                 */
    .bmRuntimeFlags = TIANOCOREDEBUG,
    .ActiveLocale = { "C", &_cdeLconv_C, NULL }/*&_cdeCLocale*/
};

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
    return &CdeAppIfUefiShellDrv;
}

static void _StdOutPutChar(int c, void** ppDest) {
#ifdef LLVM_COMPILER_WORKAROUND
    volatile size_t dummyret = (size_t)-1;
    if (c == EOF)
        dummyret = fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);
    else
        dummyret = fwrite(&c, 1, 1, (FILE*)CDE_STDOUT);
#else// LLVM_COMPILER_WORKAROUND
    if (c == EOF)
        fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);
    else
        fwrite(&c, 1, 1, (FILE*)CDE_STDOUT);
#endif//LLVM_COMPILER_WORKAROUND
}

static int _StdInGetCharSHELL(void** ppDest) {
    return fgetc((FILE*)CDE_STDIN);
}

static int _StdInGetCharDXE(void** ppDest) {
    return EOF;
}


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

/** _cdeCRT0UefiShellDrvEDK()

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiShellDrvEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
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
EFI_STATUS EFIAPI _cdeCRT0UefiShellDrvEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;

    //
    // initialize EDK global variables provided by UefiBootServicesTableLib.lib and UefiRuntimeServicesTableLib.lib
    //
    _cdegImageHandle = ImageHandle;
    _cdegST = SystemTable;
    _cdegBS = SystemTable->BootServices;
    _cdegRT = SystemTable->RuntimeServices;
    
    CdeAppIfUefiShellDrv.DriverParm.BsDriverParm.ImageHandle = ImageHandle;
    CdeAppIfUefiShellDrv.DriverParm.BsDriverParm.pSystemTable = SystemTable;

    return EFI_SUCCESS == Status ? _ModuleEntryPoint(ImageHandle, SystemTable) : Status;    // run ProcessLibraryConstructorList(), 
                                                                                            // ProcessModuleEntryPointList(), provided 
                                                                                            // in AutoGen.c ...
}

EFI_STATUS EFIAPI _cdeCRT0UefiShellDrvEDKINT3(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    __debugbreak();
    return _cdeCRT0UefiShellDrvEDK(ImageHandle, SystemTable);
}
#endif//_DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

EFI_STATUS EFIAPI _MainEntryPointUefiShellDrv(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_LOAD_ERROR;
    int i, argc;
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
    //static EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
    //CDE_LOADOPTIONS_PROTOCOL* pCdeLoadOptionsProtocol;
    char/* pLoadOptions,*/ * pLoadOptionsRW;
    size_t eflags = __cdeGetEFLAGS();

    do {

        unsigned short AcpiPmTmrBase = UINT16_MAX;

        if (1)
        {
            uint32_t FirmwareTableID = 'PCAF';
            EFI_ACPI_6_2_FIXED_ACPI_DESCRIPTION_TABLE* pFACP;
            uint32_t sizeTbl;
            char foundTbl = 0;
            static EFI_GUID EfiAcpi20TableGuid = EFI_ACPI_20_TABLE_GUID;
            EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER* pRSD = NULL;
            EFI_CONFIGURATION_TABLE* pCfg = _cdegST->ConfigurationTable;

            for (unsigned x = 0; x < _cdegST->NumberOfTableEntries; x++)
            {
                int64_t qwSig;

                if (IsEqualGUID(&EfiAcpi20TableGuid, &_cdegST->ConfigurationTable[x].VendorGuid))
                {
                    pRSD = pCfg[x].VendorTable;
                    qwSig = pRSD->Signature;
                    break;
                }
            }

            if (NULL != pRSD) do
            {
                EFI_ACPI_SDT_HEADER* pXSDT = (void*)((size_t)pRSD->XsdtAddress);
                EFI_ACPI_2_0_COMMON_HEADER* pTBLEnd = (void*)&(((char*)pXSDT)[pXSDT->Length]);
                EFI_ACPI_2_0_COMMON_HEADER** ppTBL = (void*)&(((char*)pXSDT)[sizeof(EFI_ACPI_SDT_HEADER)]);
                ptrdiff_t numTbl = pTBLEnd - (EFI_ACPI_2_0_COMMON_HEADER*)ppTBL, idxTbl;

                for (idxTbl = 0; idxTbl < numTbl; idxTbl++, ppTBL++)        // walk through all tables in the XSDT
                {                                                           //
                    if (FirmwareTableID == (*ppTBL)->Signature)             // if signatur match...
                    {                                                       //
                        pFACP = (void*)*ppTBL;                              // ... save table address
                        sizeTbl = (*ppTBL)->Length;                         // save table length
                        foundTbl = 1;                                       // mark found flag and...
                        AcpiPmTmrBase = (unsigned short)pFACP->PmTmrBlk;    //
                        break;// for()                                      // ...break for()
                    }                                                       // 
                }
            } while (0);
        }

        //
        // clock() initialization for the UEFI SHELL internal usage
        //
        CdeAppIfUefiShellDrv.pCdeServices->TSClocksAtSystemStart = gCdeServicesUefiShellDrv.pGetTsc(&CdeAppIfUefiShellDrv);
        if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
            CdeAppIfUefiShellDrv.pCdeServices->TSClocksPerSec = gCdeServicesUefiShellDrv.pGetTscPerSec(&CdeAppIfUefiShellDrv, AcpiPmTmrBase);
            CdeAppIfUefiShellDrv.pCdeServices->TimeAtSystemStart = gCdeServicesUefiShellDrv.pGetTime(&CdeAppIfUefiShellDrv);
        }
        //
        // clock() initialization for the UEFI SHELL APP usage, just copy the values
        //
        gCdeServicesCdeUefiShellApp.TSClocksAtSystemStart = CdeAppIfUefiShellDrv.pCdeServices->TSClocksAtSystemStart;
        if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
            gCdeServicesCdeUefiShellApp.TSClocksAtSystemStart = CdeAppIfUefiShellDrv.pCdeServices->TSClocksPerSec;
            gCdeServicesCdeUefiShellApp.TimeAtSystemStart = CdeAppIfUefiShellDrv.pCdeServices->TimeAtSystemStart;
        }

        pLoadOptionsRW = malloc(1 + strlen(gEfiCallerBaseName));

        strcpy(pLoadOptionsRW, gEfiCallerBaseName);

        //
        // split the driver command line up 
        //
        argc = _cdeStr2Argcv((char**)&argvex[0 + 2], pLoadOptionsRW);

        memset(&CdeAppIfUefiShellDrv.rgcbAtexit[0], 0, CDE_ATEXIT_REGISTRATION_NUM * sizeof(CdeAppIfUefiShellDrv.rgcbAtexit[0]));

        if (0 == __cdeGetCurrentPrivilegeLevel())       // running in RING0
            __CDEINTERRUPT_ENABLE;

        //
        // version string
        //
        if (2 == argc && 0 == strcmp(argvex[1 + 2], "--TOROCVER"))
        {
            fprintf(stdout, "%s\n", _gCdeStrLibcVersion);
            return 3;
        }
        //
        // install notify callback triggered by CdeEfiShellProtocol availability
        //
        EfiCreateProtocolNotifyEvent(
            &_gCdeEfiShellProtocolGuid,
            TPL_CALLBACK,
            notifyCB,
            NULL,
            &pShellProtocolRegistration
        );

        //
        // invoke main(argc, argv)
        //
        Status = setjmp(CdeAppIfUefiShellDrv.exit_buf) ? CdeAppIfUefiShellDrv.exit_status : main(argc, (char**)&argvex[0 + 2]);

        if (0 == __cdeGetCurrentPrivilegeLevel())       // running in RING0
            if(0 == (0x200 & eflags))                   // restore IF interrupt flag
                __CDEINTERRUPT_DISABLE;

        for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--)
            if (NULL != CdeAppIfUefiShellDrv.rgcbAtexit[i])
                (*CdeAppIfUefiShellDrv.rgcbAtexit[i])();

        //
        // free memory allocated during runtime
        //
        if (CDE_FREE_MEMORY_ALLOCATION_ON_EXIT)
        {
            HEAPDESC* pHeap = &CdeAppIfUefiShellDrv.pCdeServices->HeapStart;
            do
            {
                if (pHeap->qwMagic == ALLOCMEM)
                {
                    free(&pHeap[1]);
                    pHeap = &CdeAppIfUefiShellDrv.pCdeServices->HeapStart;
                }
                pHeap = pHeap->pSucc;
            } while (0 > (ptrdiff_t)pHeap);

        }//if( CDE_FREE_MEMORY_ALLOCATION_ON_EXIT ) 

        Status = EFI_SUCCESS;

    } while (0);

    return Status;
}

/**

Synopsis
    void __cdeFatalCdeServicesNotAvailUefiShellDrv(void);
Description
    Function is invoked if CdeServices not found and writes error message via ReportStatusCode.

Returns

    @param[in]  PeiServices

    @retval

**/
static void __cdeFatalCdeServicesNotAvailUefiShellDrv(EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;
    static EFI_GUID EfiStatusCodeRuntimeProtocolGuid = { 0xD2B2B828, 0x0826, 0x48A7, { 0xB3, 0xDF, 0x98, 0x3C, 0x00, 0x60, 0x24, 0xF0 } };
    EFI_REPORT_STATUS_CODE* pReportStatucCode;

    typedef struct _DEBUGNFO {
        EFI_STATUS_CODE_DATA Header;
        unsigned  ErrorLevel;
        unsigned long long            parm[12];
        char             szText[sizeof("\r\n\r\nFATAL ERROR : CdeServices UEFISHELLDRV not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n")];
    }DEBUGNFO;


    static DEBUGNFO DbgNfo = {
                    .Header.HeaderSize = sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Size = sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Type = { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
                    .ErrorLevel = 0xFF,
                    .parm = {0,0,0,0,0,0,0,0,0,0,0,0},
                    .szText = "\r\n\r\nFATAL ERROR : CdeServices UEFISHELLDRV not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n"
    };

    //
    // locate the protocols needed to run CdeLib
    //
    Status = SystemTable->BootServices->LocateProtocol(&EfiStatusCodeRuntimeProtocolGuid, NULL, (void**)&pReportStatucCode);

    if(EFI_SUCCESS == Status)
        (*pReportStatucCode)(EFI_DEBUG_CODE, (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED), 0, NULL, (void*)&DbgNfo);
}

//
// UefiMain entry hook
//
VOID* pShellProtocolRegistration = NULL;

static /*_PUTCHAR*/ void shellputc(int c, void** ppDest)
{
    wchar_t wc[] = { L'\0','\0' };

    if ('\n' == c)
        shellputc('\r', ppDest);

    wc[0] = (wchar_t)c;

    _cdegST->ConOut->OutputString(_cdegST->ConOut, wc);
}

static /*_GETCHAR*/ int shellgetc(void** ppSrc)
{
    EFI_INPUT_KEY key;
    _cdegST->ConIn->ReadKeyStroke(_cdegST->ConIn, &key);

    return key.UnicodeChar;
}


static void notifyCB(EFI_EVENT Event, VOID* Context)
{
    static int cntCB = 0;/* count call back -- Kick the event so we will perform an initial pass of current installed drivers*/
    EFI_STATUS Status;
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    char* pDst, * pSrc;
    ptrdiff_t len;
    UINT64 Position;

    if (0 != cntCB++)
    {
        //
        // get the ReportStatusCodeProtocol
        //
        Status = _cdegBS->LocateProtocol(&_gEfiStatusCodeRuntimeProtocolGuid, NULL, (void**)&gCdeServicesCdeUefiShellApp.ReportStatusCode.pShell);
        Status = _cdegBS->LocateProtocol(&_gCdeEfiShellProtocolGuid, NULL, (void**)&gCdeServicesCdeUefiShellApp.pvEfiShellProtocol);

        //
        // extend current pCdeAppIf (DXE) to full interface set (SHELL), while running
        // NOTE: Copy timer data, HEAPDESC data and so on from old gCdeServicesUefiShellDrv (DXE)
        //       to new gCdeServicesCdeUefiShellApp (SHELL) and 


        /*  copy initialized data area from gCdeServicesUefiShellDrv to gCdeServicesCdeUefiShellApp
        
            copy start-->   typedef struct _CDE_SERVICES {
                                unsigned short wVerMajor;
                                unsigned short wVerMinor;
                                unsigned char fx64Opcode;
                                unsigned char fMemoryDiscovered;
                                HEAPDESC HeapStart;

                                unsigned long long TSClocksAtSystemStart;
                                unsigned long long TSClocksAtCdeTrace;
                                unsigned long long TSClocksPerSec;

                                long long TimeAtSystemStart;
            copy end-->
                            REPORT_STATUS_CODE ReportStatusCode;
                            .
                            .
                            .
        */
        pSrc = (void*)&gCdeServicesUefiShellDrv;
        pDst = (void*)&gCdeServicesCdeUefiShellApp;
        len = (char*)&pCdeAppIf->pCdeServices->ReportStatusCode - (char*)pCdeAppIf->pCdeServices;
        memcpy(pDst, pSrc, len);

        pCdeAppIf->pCdeServices = &gCdeServicesCdeUefiShellApp;
        pCdeAppIf->cIob = CDE_FILEV_MAX;
        pCdeAppIf->DriverParm.BsDriverParm.OSIf = SHELLIF;  // enable fprint()

        pCdeAppIf->pCdeServices->pPutConOut = _StdOutPutChar;
        pCdeAppIf->pCdeServices->pGetConIn = _StdInGetCharSHELL;

        CDE_STDIN->pRootProtocol = &FileInterfaceStdIn;
        CDE_STDIN->pFileProtocol = &FileInterfaceStdIn;
        CDE_STDIN->fRsv = TRUE;
        CDE_STDIN->openmode = O_TEXT + O_RDONLY + O_CDESTDIN + O_CDENOSEEK + (EFI_UNSUPPORTED == _iob[0].pFileProtocol->GetPosition(_iob[0].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
        CDE_STDIN->emufp = &_iob[0];

        CDE_STDOUT->pRootProtocol = &FileInterfaceStdOut;
        CDE_STDOUT->pFileProtocol = &FileInterfaceStdOut;
        CDE_STDOUT->fRsv = TRUE;
        CDE_STDOUT->openmode = O_TEXT + O_WRONLY + O_CDESTDOUT + (pCdeAppIf->STDOUT816BitMode == 0 ? O_CDENOSEEK : 0) + (EFI_UNSUPPORTED == _iob[1].pFileProtocol->GetPosition(_iob[1].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
        CDE_STDOUT->emufp = &_iob[1];

        CDE_STDERR->pRootProtocol = &FileInterfaceStdErr;
        CDE_STDERR->pFileProtocol = &FileInterfaceStdErr;
        CDE_STDERR->fRsv = TRUE;
        CDE_STDERR->openmode = O_TEXT + O_WRONLY + O_CDESTDERR + (pCdeAppIf->STDOUT816BitMode == 0 ? O_CDENOSEEK : 0) + (EFI_UNSUPPORTED == _iob[2].pFileProtocol->GetPosition(_iob[2].pRootProtocol, &Position) ? O_CDEWCSZONLY : O_CDEREDIR);
        CDE_STDERR->emufp = &_iob[2];

        //
        // install the CdeServices for SHELLAPP
        //
        Status = _cdegBS->InstallMultipleProtocolInterfaces(
            &_cdegImageHandle,                                      // Image Handle
            &_gCdeShellProtocolGuid, &gCdeServicesCdeUefiShellApp,  // Guid / Protocol pair
            NULL                                                    // End of list
        );
        CDETRACE((CDEINF(1) "--> Shell Protocol available now ...\n"));

    }
}

/** UefiMain()

Synopsis
    EFI_STATUS UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
Description

    UefiMain is the entry point name defined by EDK2 for the UEFI Shell.
    This symbol (UefiMain) is hijacked to inject CdePkg-feature-set at start.

    UefiMain() is invoked by EDK2-build-autogenerated function ProcessModuleEntryPointList() (AutoGen.c).
    The original "UefiMain()" function name from SHELL.c must be renamed to "UefiMainCDEHOOKED()" by macro 
    substituion (e.g. "#define UefiMain=UefiMainCDEHOOKED" or adding "UefiMain=UefiMainCDEHOOKED" to the compilers
    preprocessor definitions.)

    @param[in]  EFI_HANDLE ImageHandle
    @parm[in]   EFI_SYSTEM_TABLE* SystemTable

    @retval EFI_STATUS

**/
EFI_STATUS UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
    EFI_LOADED_IMAGE_PROTOCOL* pLoadedImageProtocol;
    
    SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&pLoadedImageProtocol);

    if (0 == wcsncmp(L"Shell.efi -exit", pLoadedImageProtocol->LoadOptions, wcslen(L"Shell.efi -exit")))
    {
        _gCdeRTUefiShellInstanceType = 1;

        return  UefiMainCDEHOOKED(ImageHandle, SystemTable);//kgtest

    }

    pCdeAppIf->pCdeServices->pPutConOut = shellputc;                    // redirect serial CDETRACE() output to Shell CONOUT
    pCdeAppIf->pCdeServices->pGetConIn = shellgetc;                     // redirect serial CDETRACE() output to Shell CONOUT

    _gCdeRTUefiShellInstanceType = 0;

    return _MainEntryPointUefiShellDrv(ImageHandle, SystemTable);    // invoke the CRT0
}

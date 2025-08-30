/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiDxeEntryPointEDK.c

Abstract:

    CRT0 for UEFI DXE in the EDK build environment

Author:

    Kilian Kegel

--*/
#define OS_EFI

#include <stddef.h>

#undef NULL
#define ASSERT
#include <cde.h>
#include <CdeServices.h>

#include <Protocol/smmbase2.h>
//
// setjmp.h
//
extern __declspec(dllimport) int setjmp(jmp_buf);

//
// prototypes
//
static void __cdeFatalCdeServicesNotAvailDXE(EFI_SYSTEM_TABLE* SystemTable);

//
// externals
//
EFI_HANDLE               _cdegImageHandle;
EFI_SYSTEM_TABLE*        _cdegST;
EFI_BOOT_SERVICES*       _cdegBS;
EFI_RUNTIME_SERVICES*    _cdegRT;

extern int main(int argc, char** argv);
extern void _cdeSigDflt(int sig);
extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;
extern struct lconv _cdeLconv_C;
extern GUID gEfiCallerIdGuid;
extern int _cdeStr2Argcv(char** argv, char* szCmdline);
extern char __cdeGetCurrentPrivilegeLevel(void);
extern size_t _cdeInt2EfiStatus(int intstatus);
extern void __cde80387FINIT(void);

extern EFI_GUID gEfiLoadedImageProtocolGuid;
extern EFI_GUID _gCdeDxeProtocolGuid;                   // The GUID for the protocol

extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);

//
// globals
//
static CDE_APP_IF CdeAppIfDxe = {

    .nErrno = 0,
    .nEfiErrno = 0,
    
    .pCdeServices = NULL,

    .DriverParm.BsDriverParm.OSIf = DXEIF,
    .DriverParm.BsDriverParm.ImageHandle = 0,
    .DriverParm.BsDriverParm.pSystemTable = 0,

    .pStrtokStaticInternal = 0,
    .nNextRandStaticInternal = 0,
    .exit_buf = {0},
    .exit_status = EFI_SUCCESS,
    .rgcbAtexit = {0},
    .lTmpNamNum = 0,
    .rgfnSignal = {_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt},
    .pIob = NULL,   // &_iob[0],
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
    return &CdeAppIfDxe;
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

/** _cdeCRT0UefiDxeEDK()

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiDxeEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
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
EFI_STATUS EFIAPI _cdeCRT0UefiDxeEDK(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;

    //
    // initialize EDK global variables provided by UefiBootServicesTableLib.lib and UefiRuntimeServicesTableLib.lib
    //
    _cdegImageHandle = ImageHandle;
    _cdegST = SystemTable;
    _cdegBS = SystemTable->BootServices;
    _cdegRT = SystemTable->RuntimeServices;

    do {

        //
        // get the CdeServices
        //
        Status = SystemTable->BootServices->LocateProtocol(&_gCdeDxeProtocolGuid, NULL, (void*)&CdeAppIfDxe.pCdeServices);
        if (EFI_SUCCESS != Status)
        {
            __cdeFatalCdeServicesNotAvailDXE(SystemTable);
            break;//do{}while(0);
        }

        CdeAppIfDxe.DriverParm.BsDriverParm.ImageHandle = ImageHandle;
        CdeAppIfDxe.DriverParm.BsDriverParm.pSystemTable = SystemTable;

    } while (0);

    return EFI_SUCCESS == Status ? _ModuleEntryPoint(ImageHandle, SystemTable) : Status;
}

EFI_STATUS EFIAPI _cdeCRT0UefiDxeEDKINT3(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    __debugbreak();
    return _cdeCRT0UefiDxeEDK(ImageHandle, SystemTable);
}
#endif//_DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

//
// prototype
//
static void __cdeFatalCdeServicesNotAvailDXE(EFI_SYSTEM_TABLE* SystemTable);

EFI_STATUS EFIAPI _MainEntryPointDxe(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_LOAD_ERROR;
    int i, argc, nRet;
    static void* argvex[CDE_ARGV_MAX + 2];

    argvex[0] = (void*)ImageHandle;
    argvex[1] = (void*)SystemTable;//ADD SUPPORT FOR argv[-1] argv[-2]

    //
    // LoadOptions / command line interface
    //
    static EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
    CDE_LOADOPTIONS_PROTOCOL* pCdeLoadOptionsProtocol;
    char* pLoadOptions, * pLoadOptionsRW;
    size_t eflags = __cdeGetEFLAGS();

    //
    // FPU Coprocessor 80387 initialization
    // 
    __cde80387FINIT();

    do {

        //
        // get the LoadOptions / command line from the LoadOptions driver
        //
        if (1)
        {
            char fLoadOptionsAvail = FALSE;

            if (EFI_SUCCESS == SystemTable->BootServices->LocateProtocol(&gCdeLoadOptionsProtocolGuid, NULL, (void**)&pCdeLoadOptionsProtocol))
            {
                pLoadOptions = pCdeLoadOptionsProtocol->pGetLoadOptions((void*)SystemTable, (COMM_GUID*)&gEfiCallerIdGuid, (char*)&CdeAppIfDxe.szTmpBuf);
                
                if (NULL != pLoadOptions)/*program name is not registered in the LoadOptions driver?*/
                    fLoadOptionsAvail = TRUE;
            }
            
            if (TRUE == fLoadOptionsAvail)
            {
                pLoadOptionsRW = malloc(1 + strlen(pLoadOptions));
                strcpy(pLoadOptionsRW, pLoadOptions);
            }
            else
            {
                pLoadOptionsRW = malloc(1 + strlen(gEfiCallerBaseName));
                strcpy(pLoadOptionsRW, gEfiCallerBaseName);
            }
        }

        //
        // split the driver command line up 
        //
        argc = _cdeStr2Argcv((char**)&argvex[0 + 2], pLoadOptionsRW);

        memset(&CdeAppIfDxe.rgcbAtexit[0], 0, CDE_ATEXIT_REGISTRATION_NUM * sizeof(CdeAppIfDxe.rgcbAtexit[0]));

        if (0 == __cdeGetCurrentPrivilegeLevel())       // running in RING0
            __CDEINTERRUPT_ENABLE;

        nRet = setjmp(CdeAppIfDxe.exit_buf) ? CdeAppIfDxe.exit_status : main(argc, (char**)&argvex[0 + 2]);

        Status = _cdeInt2EfiStatus(nRet);

        if (0 == __cdeGetCurrentPrivilegeLevel())       // running in RING0
            if(0 == (0x200 & eflags))                   // restore IF interrupt flag
                __CDEINTERRUPT_DISABLE;

        for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--)
            if (NULL != CdeAppIfDxe.rgcbAtexit[i])
                (*CdeAppIfDxe.rgcbAtexit[i])();

        //
        // free memory allocated during runtime
        //
        if (CDE_FREE_MEMORY_ALLOCATION_ON_EXIT)
        {
            HEAPDESC* pHeap = &CdeAppIfDxe.pCdeServices->HeapStart;
            do
            {
                if (pHeap->qwMagic == ALLOCMEM)
                {
                    free(&pHeap[1]);
                    pHeap = &CdeAppIfDxe.pCdeServices->HeapStart;
                }
                pHeap = pHeap->pSucc;
            } while (pHeap);

        }//if( CDE_FREE_MEMORY_ALLOCATION_ON_EXIT ) 

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
static void __cdeFatalCdeServicesNotAvailDXE(EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;
    static EFI_GUID EfiStatusCodeRuntimeProtocolGuid = { 0xD2B2B828, 0x0826, 0x48A7, { 0xB3, 0xDF, 0x98, 0x3C, 0x00, 0x60, 0x24, 0xF0 } };
    EFI_REPORT_STATUS_CODE* pReportStatucCode;

    typedef struct _DEBUGNFO {
        EFI_STATUS_CODE_DATA Header;
        unsigned  ErrorLevel;
        unsigned long long            parm[12];
        char             szText[sizeof("\r\n\r\nFATAL ERROR : CdeServices DXE not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n")];
    }DEBUGNFO;


    static DEBUGNFO DbgNfo = {
                    .Header.HeaderSize = sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Size = sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Type = { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
                    .ErrorLevel = 0xFF,
                    .parm = {0,0,0,0,0,0,0,0,0,0,0,0},
                    .szText = "\r\n\r\nFATAL ERROR : CdeServices DXE not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n"
    };

    //
    // locate the protocols needed to run CdeLib
    //
    Status = SystemTable->BootServices->LocateProtocol(&EfiStatusCodeRuntimeProtocolGuid, NULL, (void**)&pReportStatucCode);

    if(EFI_SUCCESS == Status)
        (*pReportStatucCode)(EFI_DEBUG_CODE, (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED), 0, NULL, (void*)&DbgNfo);
}

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
static void __cdeFatalCdeServicesNotAvailable(EFI_SYSTEM_TABLE* SystemTable);
static int _StdInGetCharDXE(void** ppDest);
static int _StdInGetCharSHELL(void** ppDest);
static void _StdOutPutChar(int c, void** ppDest);

EFI_STATUS main(int argc, char** argv);

//
// externals
//
extern EFI_GUID _gCdeShellProtocolGuid;
extern void _disable(void);
extern void _enable(void);
extern void _cdeSigDflt(int sig);
extern struct lconv _cdeLconv_C;

#pragma intrinsic (_disable, _enable)


//
// globals
//
char* gszCdeDriverName;
static CDEFILE _iob[CDE_FILEV_MAX];                         // Microsoft definition. It must be buildable within the DDK
static void* argvex[CDE_ARGV_MAX + 2];                      // extended ARGV NOTE: argvex[-1], argvex[-2] userd for IMAGE_HANDLE and EFI_SYSTEM_TABLE
EFI_HANDLE _cdegImageHandle;
EFI_SYSTEM_TABLE* _cdegST;
EFI_BOOT_SERVICES* _cdegBS;
EFI_RUNTIME_SERVICES* _cdegRT;

static CDE_APP_IF CdeAppIf = {                              // CdeAppIf static duration

    .nErrno = 0,
    .nEfiErrno = 0,

    .pCdeServices = NULL,   
    {
        .BsDriverParm.OSIf = SHELLIF,
        .BsDriverParm.ImageHandle = 0,
        .BsDriverParm.pSystemTable = 0
    },
    .pStrtokStaticInternal = 0,
    .nNextRandStaticInternal = 0,
    .exit_buf = { 0 },
    .exit_status = EFI_SUCCESS,
    .rgcbAtexit = { 0 },
    .lTmpNamNum = 0,
    .rgfnSignal = { _cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt },
    .pIob = &_iob[0],
    .cIob = CDE_FILEV_MAX,
    .pArgvEx = &argvex[0],                      // pointer to +2extended argv[]
    .cArgvEx = CDE_ARGV_MAX + 2,                // number of pointers

    .bmRuntimeFlags = 0,    /* clear all runtimeflags   */
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
    return &CdeAppIf;
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
EFI_STATUS EFIAPI _cdeCRT0CdeUefiShellApp(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
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
        Status = _cdegBS->LocateProtocol(&_gCdeShellProtocolGuid, NULL, &CdeAppIf.pCdeServices);
        if (EFI_SUCCESS != Status)
        {
            __cdeFatalCdeServicesNotAvailable(SystemTable);
            break;//do{}while(0);
        }

    } while (0);

    CdeAppIf.DriverParm.BsDriverParm.ImageHandle = ImageHandle;
    CdeAppIf.DriverParm.BsDriverParm.pSystemTable = SystemTable;

    Status = CdeAppIf.pCdeServices->pCRT0Service(
        &CdeAppIf, 
        NULL, NULL, NULL    /* this is _NOT_ an EDK2 driver, but a CDEDriver */
    );
    return Status;
}
//    return EFI_SUCCESS == Status ? _MainEntryPointCdeUefiShellApp(ImageHandle, SystemTable) : Status;
//                                                                                            // do not run ProcessLibraryConstructorList(), 
//                                                                                            // ProcessModuleEntryPointList(), provided 
//                                                                                            // in AutoGen.c ...

//EFI_STATUS EFIAPI _cdeCRT0CdeUefiShellAppEDKINT3(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
//{
//    __debugbreak();
//    return _cdeCRT0CdeUefiShellAppEDK(ImageHandle, SystemTable);
//}
#endif//_DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

/**

Synopsis
    void __cdeFatalCdeServicesNotAvailable(EFI_SYSTEM_TABLE* SystemTable);
Description
    Function is invoked if CdeServices not found and writes error message via ReportStatusCode.

Returns

    @param[in]  EFI_SYSTEM_TABLE* SystemTable

    @retval

**/
static void __cdeFatalCdeServicesNotAvailable(EFI_SYSTEM_TABLE* SystemTable)
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

/*static void sendc(int c)
{
    if ('\n' == c)
        sendc('\r');
    while (0x60 != (0x60 & inp(0x3FD)))
        ;
    outp(0x3F8, c);
}

static void sends(char* s)
{
    while (*s)
        sendc(*s++);
}
//sends(__FILE__"\\"__FUNCTION__"(): 3\n");/**/

//
// UefiMain entry hook
//

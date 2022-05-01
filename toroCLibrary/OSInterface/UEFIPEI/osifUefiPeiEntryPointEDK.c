/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiPeiEntryPointEDK.c

Abstract:

    CRT0 for PEI in the EDK build environment

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>
#include <cde.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>

//
// prototypes
//
static void __cdeFatalCdeServicesNotAvailPEI(const EFI_PEI_SERVICES** PeiServices);

//
// externals
//
extern int main(int argc, char** argv);
extern void _cdeSigDflt(int sig);
extern struct _CDE_LCONV_LANGUAGE _locale_C_;
extern GUID gEfiCallerIdGuid;
extern char* gEfiCallerBaseName;
extern int _cdeStr2Argcv(char** argv, char* szCmdline);
extern char __cdeGetCurrentPrivilegeLevel(void);
extern void* __cdeGetPeiServices(void);

//
// 
//
typedef struct _CDE_APP_IF_HOB {
    EFI_HOB_GUID_TYPE       HobGuidType;
    EFI_PEI_PPI_DESCRIPTOR  PpiDesc;
    CDE_APP_IF              CdeAppIf;
}CDE_APP_IF_HOB;

static CDE_APP_IF_HOB CdeAppIfHobRomDflt =
{
    .HobGuidType.Header = { EFI_HOB_TYPE_GUID_EXTENSION, sizeof(CDE_APP_IF_HOB), 0},
    .HobGuidType.Name = CDE_APP_IF_HOB_GUID,

    .PpiDesc.Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    .PpiDesc.Guid = &gEfiCallerIdGuid,  // NOTE:   gEfiCallerIdGuid.Data1 will be patched to CDEA1Fxx
                                        //         to identify Applicatio1nterFace at runtime
    .PpiDesc.Ppi = 0                    /* initialize at runtime */,

    .CdeAppIf.DriverParm.PeimParm.OSIf = PEIIF,
    .CdeAppIf.exit_status = EFI_SUCCESS,
    .CdeAppIf.rgfnSignal = {_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt},
    .CdeAppIf.pIob = (CDEFILE*)-1,
    .CdeAppIf.cIob = 0,
    .CdeAppIf.bmRuntimeFlags = TIANOCOREDEBUG,
    .CdeAppIf.pActiveLocale = &_locale_C_,
};
///
// globals
//
static EFI_GUID gCdePeiProtocolGuid = CDE_PEI_PROTOCOL_GUID;    // The GUID for the protocol
static EFI_PEI_SERVICES** gPeiServices4EMULATION = NULL;        // in EMULATION PeiServices can't be retrieved at IDT location

/** __cdeGetAppIf()

Synopsis
    void* __cdeGetAppIf(void);
Description
    
    Function locates the "Application Interface" (r/w memory) that is exclusive for each driver
    at runtime.
    For PeiDrivers this chunk of memory is placed in a HOB, allocated at startup

    Returns

    @param[in]  void

    @retval (void*)pCdeAppIfPei

**/
void* __cdeGetAppIf(void)
{
    EFI_GUID AppIfGuid = gEfiCallerIdGuid;
    EFI_PEI_SERVICES** PeiServices;
    CDE_APP_IF* pCdeAppIfPei/*return NULL in case of CdeAppIf is n/a*/;
    EFI_STATUS Status;

    //
    // patch the AppIfGuid to CDEA1Fxx.xxxx.xxxx.xx.xx.xx.xx.xx.xx.xx.xx
    //
    //AppIfGuid.Data1 &= 0xFF;        //TODO
    //AppIfGuid.Data1 |= 0xCDEA1F00;  //TODO

    //
    // get PeiServices first ...
    //
    if (0 == __cdeGetCurrentPrivilegeLevel()) {
        PeiServices = __cdeGetPeiServices();    // https://uefi.org/sites/default/files/resources/PI_Spec_1_7_A_final_May1.pdf#page=89
    }
    else {
        PeiServices = gPeiServices4EMULATION;   // in EMULATION PeiServices can't be retrieved at IDT location
    }

    //
    // ... to be able to locate the CdeAppIf protocol
    //
    Status = (*PeiServices)->LocatePpi(PeiServices, &AppIfGuid, 0, NULL, (void**)&pCdeAppIfPei);

    if (Status != EFI_SUCCESS)
    {
        volatile int deadloop = 0;
    
        _outp(0x80, 0xCA/*TASTROPHE*/);

        while (0 == deadloop)
            ;
    }
    
    return EFI_SUCCESS == Status ? pCdeAppIfPei : NULL/*return NULL in case of CdeAppIf is n/a*/;
}

//
// include the original tianocore/UEFI PeimEntryPoint.c 
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
#include "..\EDK2ObjBlocker\PeimEntryPoint_c.h"
#include "..\EDK2ObjBlocker\DebugLib_c.h"

#ifndef _DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

/** _cdeCRT0UefiPeiEDK()

Synopsis
    #include <CdeServices.h>
    EFI_STATUS EFIAPI _cdeCRT0UefiPeiEDK(IN EFI_PEI_FILE_HANDLE FileHandle, IN CONST EFI_PEI_SERVICES** PeiServices);
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
EFI_STATUS EFIAPI _cdeCRT0UefiPeiEDK(IN EFI_PEI_FILE_HANDLE FileHandle, IN CONST EFI_PEI_SERVICES** PeiServices)
{
    EFI_STATUS Status = EFI_SUCCESS;
    CDE_APP_IF* pCdeAppIf;
    CDE_SERVICES* pCdeServices;
    CDE_APP_IF_HOB* pCdeAppIfHob = &CdeAppIfHobRomDflt;
    int i;

    do {

        Status = (*PeiServices)->LocatePpi(PeiServices, &gCdePeiProtocolGuid, 0, NULL, (void**)&pCdeServices);
        if (EFI_SUCCESS != Status)
        {
            __cdeFatalCdeServicesNotAvailPEI(PeiServices);
            break;
        }

        //
        // store PeiServices in a static duration to w/a the PEI Services Table Retrieval for EMULATION
        //      https://uefi.org/sites/default/files/resources/PI_Spec_1_7_A_final_May1.pdf#page=89
        //
        if (0 != __cdeGetCurrentPrivilegeLevel())           // running outside RING0
            gPeiServices4EMULATION = (EFI_PEI_SERVICES**)PeiServices; // static memory is writeable

        //
        // allocate HOB space, copy and initialize the CdeAppIf to publish as protocol
        //
        if (1)
        {
            //
            // allocate HOB space to get a small amount of r/w memory in PEI for placing the CDE protocol in there
            //
            Status = (*PeiServices)->CreateHob(PeiServices, EFI_HOB_TYPE_GUID_EXTENSION, sizeof(CDE_APP_IF_HOB), &pCdeAppIfHob);
            if (EFI_SUCCESS != Status)
                break;
            //pCdeAppIfHob->HobGuidType.Name = gEfiCallerIdGuid;

            //
            // initialize that memory / copy predefined settings from ROM to CAR cache as ram
            //
            for (i = 0; i < sizeof(CdeAppIfHobRomDflt); i++)
                ((char*)pCdeAppIfHob)[i] = ((char*)&CdeAppIfHobRomDflt)[i];

            pCdeAppIfHob->PpiDesc.Ppi = &pCdeAppIfHob->CdeAppIf;

            //
            // initialize remaining CdeAppIf structure in runtime HOB space
            //
            pCdeAppIf = &pCdeAppIfHob->CdeAppIf;
            pCdeAppIf->pCdeServices = pCdeServices;

            pCdeAppIf->DriverParm.PeimParm.FileHandle = FileHandle;
            pCdeAppIf->DriverParm.PeimParm.PeiServices = (EFI_PEI_SERVICES**)PeiServices;

            //
            // patch the .PpiDesc.Guid to CDEA1Fxx.xxxx.xxxx.xx.xx.xx.xx.xx.xx.xx.xx
            //
            //pCdeAppIfHob->PpiDesc.Guid->Data1 &= 0xFF;          //kgtest
            //pCdeAppIfHob->PpiDesc.Guid->Data1 |= 0xCDEA1F00;    //kgtest

            //
            // install the CdeAppIfProtocol (CDE application interface protocol)
            //
            Status = (*PeiServices)->InstallPpi(PeiServices, &pCdeAppIfHob->PpiDesc);
            if (EFI_SUCCESS != Status)
                break;
            pCdeAppIf = __cdeGetAppIf();
        }

    }while (0);

    return EFI_SUCCESS == Status ? _ModuleEntryPoint(FileHandle, PeiServices) : Status;
}

EFI_STATUS EFIAPI _cdeCRT0UefiPeiEDKINT3(IN EFI_PEI_FILE_HANDLE FileHandle, IN CONST EFI_PEI_SERVICES** PeiServices)
{
    __debugbreak();
    return _cdeCRT0UefiPeiEDK(FileHandle, PeiServices);
}
#endif//_DONT_USE_EDK2_MODULEENTRYPOINT_OVERRIDE_

EFI_STATUS
EFIAPI
_MainEntryPointPei(
    IN EFI_PEI_FILE_HANDLE       FileHandle,
    IN const EFI_PEI_SERVICES** PeiServices
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    void* argvex[CDE_ARGV_MAX + 2] = { (void*)FileHandle, (void*)PeiServices };//ADD SUPPORT FOR argv[-1] argv[-2]
    //
    // LoadOptions / command line interface
    //
    static EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
    CDE_LOADOPTIONS_PROTOCOL* pCdeLoadOptionsProtocol;
    char* pLoadOptions, * pLoadOptionsRW;
    int argc,i;
    CDE_SERVICES*pCdeServices;
    CDE_APP_IF* pCdeAppIf;

    do {

        Status = (*PeiServices)->LocatePpi(PeiServices, &gCdePeiProtocolGuid, 0, NULL, (void**)&pCdeServices);

        if (EFI_SUCCESS != Status)
        {
            __cdeFatalCdeServicesNotAvailPEI(PeiServices);
            Status = EFI_PROTOCOL_ERROR;
            break;//do{}while(0);
        }

        pCdeAppIf = __cdeGetAppIf();

        //
        // get the LoadOptions / command line from the LoadOptions driver
        //
        if (1)
        {
            char fLoadOptionsAvail = FALSE;

            if (EFI_SUCCESS == (*PeiServices)->LocatePpi(PeiServices, &gCdeLoadOptionsProtocolGuid, 0, NULL, (void**)&pCdeLoadOptionsProtocol))
            {
                pLoadOptions = pCdeLoadOptionsProtocol->pGetLoadOptions((void*)PeiServices, (COMM_GUID*)&gEfiCallerIdGuid, (char*)pCdeAppIf->szTmpBuf);

                if (NULL != pLoadOptions)/*program name is not registered in the LoadOptions driver?*/
                    fLoadOptionsAvail = TRUE;
            }

            if (TRUE == fLoadOptionsAvail)
            {
                //
                // if pCdeLoadOptionsProtocol is available AND a command line pLoadOptions for that particular gEfiCallerIdGuid is present
                //
                pLoadOptionsRW = malloc(1 + strlen(pLoadOptions));
                strcpy(pLoadOptionsRW, pLoadOptions);
            }
            else
            {
                //
                // else just copy the gEfiCallerBaseName to argv[0]
                //
                pLoadOptionsRW = malloc(1 + strlen(gEfiCallerBaseName));
                strcpy(pLoadOptionsRW, gEfiCallerBaseName);
            }
        }

        //
        // split the driver command line up into pieces
        //
        argc = _cdeStr2Argcv((char**)&argvex[0 + 2], pLoadOptionsRW);

        for (i = 0; i < CDE_ATEXIT_REGISTRATION_NUM; i++)
            pCdeAppIf->rgcbAtexit[i] = NULL;

        Status = setjmp(pCdeAppIf->exit_buf) ? pCdeAppIf->exit_status : main(argc, (char**)&argvex[0 + 2]);

        for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--)
            if (NULL != pCdeAppIf->rgcbAtexit[i])
                (*pCdeAppIf->rgcbAtexit[i])();

        //
        // free memory allocated during runtime
        //
        if (CDE_FREE_MEMORY_ALLOCATION_ON_EXIT)
        {
            HEAPDESC* pHeap = &pCdeAppIf->pCdeServices->HeapStart;
            do
            {
                if (pHeap->qwMagic == ALLOCMEM)
                {
                    free(&pHeap[1]);
                    pHeap = &pCdeAppIf->pCdeServices->HeapStart;
                }
                pHeap = pHeap->pSucc;
            } while (pHeap);

        }//if( CDE_FREE_MEMORY_ALLOCATION_ON_EXIT ) 

    } while (0);

    return EFI_SUCCESS;
}

/**

Synopsis
    void __cdeFatalCdeServicesNotAvailPEI(void);
Description
    Function is invoked if CdeServices not found and writes error message via ReportStatusCode.

Returns

    @param[in]  PeiServices

    @retval

**/
static void __cdeFatalCdeServicesNotAvailPEI(const EFI_PEI_SERVICES** PeiServices)
{
    typedef struct _DEBUGNFO {
        EFI_STATUS_CODE_DATA Header;
        unsigned  ErrorLevel;
        unsigned long long            parm[12];
        char             szText[sizeof("\r\n\r\nFATAL ERROR : CdeServices PEI not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n")];
    }DEBUGNFO;


    static DEBUGNFO DbgNfo = {
                    .Header.HeaderSize = sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Size = sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Type = { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
                    .ErrorLevel = 0xFF,
                    .parm = {0,0,0,0,0,0,0,0,0,0,0,0},
                    .szText = "\r\n\r\nFATAL ERROR : CdeServices PEI not available.\r\n              Check APRIORI and DEPEX settings!!!\r\n\r\n"
    };

    (*PeiServices)->ReportStatusCode(PeiServices, EFI_DEBUG_CODE, (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED), 0, NULL, (void*)&DbgNfo);
}
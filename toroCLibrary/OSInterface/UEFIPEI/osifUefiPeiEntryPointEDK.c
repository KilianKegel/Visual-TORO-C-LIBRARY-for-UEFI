/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifUefiPeiEntryPointEDK.c

Abstract:

    CRT0 for PEI in the EDK build environment

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <stddef.h>

#undef NULL
#include <cde.h>
#include <CdeServices.h>
//
// prototypes
//
static void __cdeFatalCdeServicesNotAvailPEI(const EFI_PEI_SERVICES** PeiServices);

//
// externals
//
extern int main(int argc, char** argv);
extern void _cdeSigDflt(int sig);
extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;
extern struct lconv _cdeLconv_C;
extern GUID gEfiCallerIdGuid;
extern char* gEfiCallerBaseName;
extern int _cdeStr2Argcv(char** argv, char* szCmdline);
extern char __cdeGetCurrentPrivilegeLevel(void);
extern void* __cdeGetPeiServices(void);
extern void __cde80387FINIT(void);

extern __declspec(dllimport) void* malloc(size_t size);
extern __declspec(dllimport) void free(void* ptr);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
extern __declspec(dllimport) void* memcpy(void* s1, const void* s2, size_t n);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) char* strcpy(char* pszDst, const char* pszSrc);
#undef setjmp
extern __declspec(dllimport) int setjmp(jmp_buf);
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
    .CdeAppIf.ActiveLocale = { "C", &_cdeLconv_C, NULL }/*&_cdeCLocale*/
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
    Status = (*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES**)PeiServices, &gEfiCallerIdGuid, 0, NULL, (void**)&pCdeAppIfPei);

    if (Status != EFI_SUCCESS)
    {
        volatile int deadloop = 0;
    
        _cdeOUTByte(0x80, 0xCA/*TASTROPHE*/);
        _cdeOUTByte(0x81, 0xCA/*TASTROPHE*/);
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
#include "..\EDK2ObjBlocker\_cdeStdCIntrinsics_c.h"
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
    CDE_APP_IF* pCdeAppIf = NULL;
    CDE_SERVICES* pCdeServices;
    CDE_APP_IF_HOB* pCdeAppIfHob = &CdeAppIfHobRomDflt;

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
        // allocate HOB space (if not yet before for that particular "gEfiCallerIdGuid"), copy and initialize the CdeAppIf to publish as protocol
        //
        if (1)
        {
            //
            // check if pCdeAppIf for that particular driver (AppIfGuid == gEfiCallerIdGuid) is already installed by a previous call
            //
            Status = (*PeiServices)->LocatePpi(PeiServices, &gEfiCallerIdGuid, 0, NULL, (void**)&pCdeAppIf);

            if (NULL == pCdeAppIf)
            {
                //
                // allocate HOB space to get a small amount of r/w memory in PEI for placing the CDE protocol in there
                //
                Status = (*PeiServices)->CreateHob(PeiServices, EFI_HOB_TYPE_GUID_EXTENSION, sizeof(CDE_APP_IF_HOB), (void**)&pCdeAppIfHob);
                if (EFI_SUCCESS != Status)
                    break;

                //
                // initialize that memory / copy predefined settings from ROM to CAR cache as ram
                //
                memcpy(pCdeAppIfHob, &CdeAppIfHobRomDflt, sizeof(CdeAppIfHobRomDflt));

                pCdeAppIfHob->PpiDesc.Ppi = &pCdeAppIfHob->CdeAppIf;

                //
                // initialize remaining CdeAppIf structure in runtime HOB space
                //
                pCdeAppIf = &pCdeAppIfHob->CdeAppIf;

                //
                // install the CdeAppIfProtocol (CDE application interface protocol)
                //
                Status = (*PeiServices)->InstallPpi(PeiServices, &pCdeAppIfHob->PpiDesc);
                if (EFI_SUCCESS != Status)
                    break;
            }

            pCdeAppIf->pCdeServices = pCdeServices;     // pCdeServices changes location from MemoryNonDiscovered to MemoryDiscovered
            pCdeAppIf->DriverParm.PeimParm.FileHandle = FileHandle;
            pCdeAppIf->DriverParm.PeimParm.PeiServices = (EFI_PEI_SERVICES**)PeiServices;

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
    void* argvex[CDE_ARGV_MAX + 2];         // DON'T INITIALIZE HERE, BECAUSE C COMPILER inserts memset()!!!

    //
    // LoadOptions / command line interface
    //
    static EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
    CDE_LOADOPTIONS_PROTOCOL* pCdeLoadOptionsProtocol;
    char* pLoadOptions, * pLoadOptionsRW;
    int argc,i;
    CDE_SERVICES*pCdeServices;
    CDE_APP_IF* pCdeAppIf;
    void* MemoryDiscoveredPpi;
    static EFI_GUID EfiPeiMemoryDiscoveredPpiGuid = { 0xf894643d, 0xc449, 0x42d1, {0x8e, 0xa8, 0x85, 0xbd, 0xd8, 0xc6, 0x5b, 0xde } };

    memset(argvex, 0, sizeof(argvex));      // instead do dedicated initialization that invokes __imp__memset()
    argvex[0] = (void*)FileHandle;          // instead do dedicated initialization that invokes __imp__memset()
    argvex[1] = (void*)PeiServices;         // instead do dedicated initialization that invokes __imp__memset()

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
        // FPU Coprocessor 80387 initialization
        // 
        __cde80387FINIT();

        //
        // check memory discovered
        //
        Status = (*PeiServices)->LocatePpi(PeiServices, &EfiPeiMemoryDiscoveredPpiGuid, 0, NULL, (void**)&MemoryDiscoveredPpi);

        if (EFI_SUCCESS == Status)
        {
            if (0 == pCdeServices->fMemoryDiscovered) {
                pCdeServices->fMemoryDiscovered = 1;
                pCdeServices->HeapStart = (HEAPDESC){ (void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1 };
            }
        }
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

        memset(&pCdeAppIf->rgcbAtexit[0], 0, CDE_ATEXIT_REGISTRATION_NUM * sizeof(pCdeAppIf->rgcbAtexit[0]));

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
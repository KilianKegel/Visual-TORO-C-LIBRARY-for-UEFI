/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Cde.h

Abstract:

    C Development Environment base definitions

Author:

    Kilian Kegel

--*/
#ifndef _CDE_H_
#define _CDE_H_
#include <stddef.h>
#include <assert.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////
// override the "DebugLib.h" ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CDE_DONT_OVERRIDE_DEBUG_TRACE
#define __DEBUG_LIB_H__ /*block "DebugLib.h" entirely*/
//
// Declare bits for PcdDebugPrintErrorLevel and the ErrorLevel parameter of DebugPrint()
//
#define DEBUG_INIT      0x00000001  // Initialization
#define DEBUG_WARN      0x00000002  // Warnings
#define DEBUG_LOAD      0x00000004  // Load events
#define DEBUG_FS        0x00000008  // EFI File system
#define DEBUG_POOL      0x00000010  // Alloc & Free (pool)
#define DEBUG_PAGE      0x00000020  // Alloc & Free (page)
#define DEBUG_INFO      0x00000040  // Informational debug messages
#define DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
#define DEBUG_VARIABLE  0x00000100  // Variable
#define DEBUG_BM        0x00000400  // Boot Manager
#define DEBUG_BLKIO     0x00001000  // BlkIo Driver
#define DEBUG_NET       0x00004000  // Network Io Driver
#define DEBUG_UNDI      0x00010000  // UNDI Driver
#define DEBUG_LOADFILE  0x00020000  // LoadFile
#define DEBUG_EVENT     0x00080000  // Event messages
#define DEBUG_GCD       0x00100000  // Global Coherency Database changes
#define DEBUG_CACHE     0x00200000  // Memory range cachability changes
#define DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may
                                    // significantly impact boot performance
#define DEBUG_ERROR     0x80000000  // Error

//
// Aliases of debug message mask bits
//
#define EFI_D_INIT      DEBUG_INIT
#define EFI_D_WARN      DEBUG_WARN
#define EFI_D_LOAD      DEBUG_LOAD
#define EFI_D_FS        DEBUG_FS
#define EFI_D_POOL      DEBUG_POOL
#define EFI_D_PAGE      DEBUG_PAGE
#define EFI_D_INFO      DEBUG_INFO
#define EFI_D_DISPATCH  DEBUG_DISPATCH
#define EFI_D_VARIABLE  DEBUG_VARIABLE
#define EFI_D_BM        DEBUG_BM
#define EFI_D_BLKIO     DEBUG_BLKIO
#define EFI_D_NET       DEBUG_NET
#define EFI_D_UNDI      DEBUG_UNDI
#define EFI_D_LOADFILE  DEBUG_LOADFILE
#define EFI_D_EVENT     DEBUG_EVENT
#define EFI_D_VERBOSE   DEBUG_VERBOSE
#define EFI_D_ERROR     DEBUG_ERROR

#ifdef DEBUG
#   undef DEBUG
#endif//DEBUG
#ifdef TRACE
#   undef TRACE
#endif//TRACE

extern void __cdeTianocoreDEBUGEna(void);
extern char* __cdeTianocoreDebugPrintErrolevel2Str(size_t ErrorLevel, const char* Format, ...);
#define DEBUG(Expression)   __cdeTianocoreDEBUGEna(),\
                            _CdeMofine(gEfiCallerBaseName,\
                                __FILE__,\
                                __LINE__,\
                                __FUNCTION__,\
                                /*string*/__cdeTianocoreDebugPrintErrolevel2Str Expression,\
                                /*condition*/MOFINE_CONFIG | 1,\
                                ""),\
                            DebugPrint Expression
#define TRACE DEBUG
#ifdef ASSERT_RETURN_ERROR
#   undef ASSERT_RETURN_ERROR
#endif//ASSERT_RETURN_ERROR
#define ASSERT_RETURN_ERROR(StatusParameter)
#ifdef ASSERT_EFI_ERROR
#   undef ASSERT_EFI_ERROR
#endif//ASSERT_EFI_ERROR
#define ASSERT_EFI_ERROR(StatusParameter)
//
extern void DebugPrint(size_t ErrorLevel, const char* pszFormat, ...);
extern void __cdecl _assert(char* pszExpession, char* pszFile, unsigned dwLine);

#ifdef ASSERT
#   undef ASSERT
#endif//ASSERT
#define ASSERT(Expression)  assert(Expression)

#else   //CDE_DONT_OVERRIDE_DEBUG_TRACE
#endif  //CDE_DONT_OVERRIDE_DEBUG_TRACE

//
//#TOCTRL NMOFINE
//

////////////////////////////////////////////////////////////////////////////////////////////////////////
// dump related definitions ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union _XDUMPPARM {
    unsigned reg;
    struct {
        unsigned ElmSizeMin1 : 3;       /*!<element size minus one, only 0,1,3,7                    */
        unsigned AddrSize : 3;          /*!<0,1,2,3,4 with:
                                            0 == "%016llX: "
                                            1 == "%08llX: "
                                            2 == "%04llX: "
                                            3 == "%02llX: "
                                            4 == ""                                                 */
        unsigned ElmsPerLine : 7;       /*!< nBytesPerLine minus one bytes per line - 0 == 16,   */
        unsigned NoAscii : 1;           /*!<append NO ASCII characters                              */
        unsigned BaseOfs : 1;           /*!<base and offset, offset only otherwise                  */
        unsigned NoDash : 1;            /*!<print dash "-" in between                               */
        unsigned Pitch : 8;             /*!<pitch between two consecutive elements fo size elmsize  */
    }bit;
}XDUMPPARM;

#define CDESTRINGIFY(x) #x
#define CDENUM2STRING(x) CDESTRINGIFY(x)

//
// externs
//
extern char* gEfiCallerBaseName;
extern void* __cdeGetAppIf(void);
extern int _CdeTraceNumCharsTransmitted;
//
// ANSI C Library related extentions 
//
#define __CDEC_HOSTED__ (((void *)0)/*NULL*/ != __cdeGetAppIf())	// replacement for __STDC_HOSTED__ 
extern char* strefierror(size_t errcode);           // strerror() replacement for UEFI. Convert EFI_STATUS to string

//
// CDE related library diagnostic extentions 
//
extern int _CdeMofine(char* pszDriver, char* pszFile, int nLine, char* pszFunction, char* pszClass, int fTraceEn, char* pszFormat, ...);
extern int _CdeXDump(XDUMPPARM ctrl, unsigned elmcount, unsigned long long startaddr, unsigned long long(*pfnGetElm)(unsigned long long qwAddr), unsigned (*pfnWriteStr)(char* szLine),char *pBuf, int bufsize);

//
// CDE MOdule FIle liNE (CDEMOFINE) trace support definitions
//
#define MOFINE_NDRIVER      (1 << 1)
#define MOFINE_NFILE        (1 << 2)
#define MOFINE_NLINE        (1 << 3)
#define MOFINE_NFUNCTION    (1 << 4)
#define MOFINE_NCLOCK       (1 << 5)
#define MOFINE_NSTDOUT      (1 << 6)/* stderr instead */
#define MOFINE_NCLASS       (1 << 7)/* stderr instead */
#define MOFINE_RAWFORMAT    (1 << 8)
#define MOFINE_STRIPPATH    (1 << 9)/* remove path from filename at runtime */

#define MOFINE_EXITONCOND   (1 << 10)
#define MOFINE_DEADONCOND   (1 << 11)

#define MOFINE_UEFIFMTSTR   (1 << 12)

#define MOFINE_STDOUT       (1 << 13)   /* trace to STDOUT */
#define MOFINE_STDERR       (1 << 14)   /* trace to STDERR */
#define MOFINE_STATUSCODE   (1 << 15)   /* trace to STATUSCODE */

#ifndef MOFINE_CONFIG
#   define MOFINE_CONFIG    MOFINE_STATUSCODE /* | MOFINE_NDRIVER | MOFINE_NFILE | MOFINE_NLINE | MOFINE_NFUNCTION | MOFINE_NCLOCK | MOFINE_NSTDOUT | MOFINE_NCLASS | MOFINE_RAWFORMAT */
#endif//MOFINE_CONFIG

//
// MOFINE trace conficuration macro
//
#define MFNBAR(cond)/*  bare        */ ((void *)0)/*NULL*/,                ((void *)0)/*NULL*/,    0,       ((void *)0)/*NULL*/,      /*string*/ 0,          /*condition*/MOFINE_CONFIG | (0 != (cond)),
#define MFNNON(cond)/*  no class    */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/ 0,          /*condition*/MOFINE_CONFIG | (0 != (cond)),
#define MFNINF(cond)/*  INFO        */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/"INFO>", 	/*condition*/MOFINE_CONFIG | (0 != (cond)),
#define MFNSUC(cond)/*  SUCCESS     */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/"SUCCESS>",	/*condition*/MOFINE_CONFIG | (0 != (cond)),
#define MFNWAR(cond)/*  WARNING     */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/"WARNING>",	/*condition*/MOFINE_CONFIG | (0 != (cond)),
#define MFNERR(cond)/*  ERROR       */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/"ERROR>", 	/*condition*/MOFINE_CONFIG | (0 != (cond)),
#define MFNFAT(cond)/*  FATAL       */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/"FATAL>", 	/*condition*/MOFINE_CONFIG | MOFINE_EXITONCOND | (0 != (cond)),
#define MFNASS(cond)/*  ASSERT      */ gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,/*string*/"ASSERT>", 	/*condition*/MOFINE_CONFIG | MOFINE_DEADONCOND | (0 != (cond)),

#ifndef NMOFINE
#define CDEMOFINE(cond_msg) _CdeMofine cond_msg /*MOdule-FIle-liNE COndition msg*/
////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDE MOdule FIle liNE (CDEMOFINE) trace macro
////////////////////////////////////////////////////////////////////////////////////////////////////////
//    NOTE: there is no comma placed between the MOFINE configuration macro MFNXXX() and the format string:
//
//        CDEMOFINE((MFNERR(1) /* <<<<<< NO COMMA HERE >>>>>> */ "##### Welcome to the jungle #####\n")); // ERROR
//
//      This is done to ease porting to core trace functions with different parameter layout.
//
//      For core trace functions w/o file/line/function etc. parameter, only the MFNXXX() needs to be modified,
//      not the signature of CDEMOFINE() in the sourcecode.
//
//      E.g. the UEFI DEBUG() macro is forced to pass a DebugLevel before the format string.
//
//    CDEMOFINE((MFNINF(1) "##### Welcome to the jungle #####\n"));                             // INFO
//    CDEMOFINE((MFNFAT(Status != EFI_SUCCESS)  "##### Welcome to the jungle #####\n"\n"));     // FATAL, only if Status != EFI_SUCCESS
//    CDEMOFINE((MFNSUC(Status == EFI_SUCCESS)  "##### Welcome to the jungle #####\n"\n"));     // SUCCESS, only if Status == EFI_SUCCESS
//    CDEMOFINE((MFNWAR(WARNLEVEL) "##### Welcome to the jungle #####\n"));                     // WARNING, if WARNLEVEL is enabled
//
//
#else// NMOFINE

#define CDEMOFINE(fineonerrcond_msg) ((void)0)

#endif//ndef NMOFINE

#define CDEPOSTCODE(c,v)	if(c)outp(0x80,v)
#define CDEDEADLOOP(c,v)	if(c){volatile int abcxyz = v;while(v == abcxyz);}
#define CDEDEBUGBREAK(c,v)	if(c){volatile int abcxyz = v;while(v == abcxyz)__debugbreak();}
#define CDEDBGMAGIC         0xCDEDB600                  // backdoor file pointer like stdin, stdout, stderr
#define CDEDBGMAGICMASK     0xFFFFFF00                  // backdoor file pointer like stdin, stdout, stderr
#define CDEDBG(x)           (void*)(CDEDBGMAGIC | x)    // backdoor file pointer like stdin, stdout, stderr
#   define CDEDBG_EN        1
#   define CDEDBG_EFIFMT    2

#define CDETRACE(cond,msg)  _CdeTraceNumCharsTransmitted  = fprintf((void*)(CDEDBGMAGIC | cond),"%s`%s(%d)`%s()`%s> ",gEfiCallerBaseName,__FILE__,__LINE__,__FUNCTION__,"INFO"),\
                            _CdeTraceNumCharsTransmitted += fprintf((void*)(CDEDBGMAGIC | cond),msg)

//
// CDE GUID definitions
//
//
// Global Unique ID for CDE
//
#define CDE_PEI_TMPTEST0_GUID           {0xCDE00055, 0xb0ff, 0x498b, { 0xb1, 0x7c, 0xed, 0xb3, 0xa0, 0x2e, 0x7f, 0x6e }}
#define CDE_LOAD_OPTIONS_PROTOCOL_GUID  {0xCDE00000, 0x2c09, 0x4284, { 0x90, 0xf9, 0x9a, 0x98, 0x8a, 0xbd, 0xf6, 0xfb }}
#define CDE_PEI_PROTOCOL_GUID           {0xCDE00001, 0xb0ff, 0x498b, { 0xb1, 0x7c, 0xed, 0xb3, 0xa0, 0x2e, 0x7f, 0x6e }}
#define CDE_DXE_PROTOCOL_GUID           {0xCDE00002, 0xe988, 0x4697, { 0x8f, 0x36, 0x08, 0xf1, 0x3d, 0x8d, 0x3d, 0x39 }}
#define CDE_SMM_PROTOCOL_GUID           {0xCDE00003, 0xee5c, 0x4079, { 0xac, 0x93, 0xee, 0x48, 0xe2, 0x1d, 0x2a, 0x99 }}
#define CDE_HOB_GUID                    {0xCDE00004, 0x8801, 0x4cfb, { 0xb1, 0xca, 0xdc, 0x63, 0xde, 0xaa, 0x52, 0xdd }}
#define CDEPKG_TOKEN_SPACE_GUID         {0xCDE00005, 0x31d3, 0x40f5, { 0xb1, 0x0c, 0x53, 0x9b, 0x2d, 0xb9, 0x40, 0xcd }}
#define CDE_END_OF_DXE_GUID             {0xCDE00006, 0x0c2a, 0x4cb4, { 0x82, 0xe4, 0x5a, 0x0b, 0x6f, 0x2f, 0x5e, 0xf2 }}
#define CDE_UNKNOWN_DRIVER_FILE_GUID	{0xCDE00007, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}
#define CDE_APP_IF_HOB_GUID             {0xCDE00008, 0xaa95, 0x48af, { 0xb5, 0xac, 0xb9, 0x32, 0x33, 0xdd, 0xcd, 0x69 }}
#define CDE_UNKNOWN_DRIVER_FILE_NAME	"CDE_UNKNOWN_DRIVER_FILE_NAME"

//
// LoadOptions / command line support
//
typedef struct _COMM_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} COMM_GUID;

typedef struct _COMMANDLINE {
    int rejectStart;            //  1 -> suppress start of driver, even if registered with EFI_CALLER_ID_GUID. 0 -> start driver and pass command line to it
    COMM_GUID EfiCallerIdGuid;	/*	EFI_CALLER_ID_GUID from AutoGen.h
                                    gEfiCallerIdGuid from AutoGen.c
                                    FILE_GUID from .INF	*/
    char* szCommandLine;		/*  assigned command line includeing filename*/
}COMMANDLINE;

typedef char* GETLOADOPTIONS (void* PeiDxeInterface, COMM_GUID * pEfiCallerIdGuid, char *pVarBuf/* of 128 elements */);

typedef struct _CDE_LOADOPTIONS_PROTOCOL {

    GETLOADOPTIONS* pGetLoadOptions;

}CDE_LOADOPTIONS_PROTOCOL;

#endif//_CDE_H_

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    osifWinNTEntryPoint.c

Abstract:

    CRT0 for Windows NT

Author:

    Kilian Kegel

--*/
//#define _STDSTREAM_DEFINED
#include <stdio.h>
#include <string.h>
#include <windows.h>
#pragma warning( disable : 4702 )
#define ALIENOS

#include <signal.h>

#include <cde.h>
#include <CdeServices.h>
//
// setjmp.h
//
extern __declspec(dllimport) int setjmp(jmp_buf);

const char _1 = 1;
const char _0 = 0;
static CDEFILE _iob[CDE_FILEV_MAX];                                  /* Microsoft definition. It must be buildable within the DDK*/
char* gszCdeDriverName;

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
extern void free(void*);
extern void (*__cdeChkStkAddr)(size_t);
extern void __chkstkWindows(size_t);

extern VWXPRINTF        _cdeCoreVwxPrintf;
extern VWXSCANF         _cdeCoreVwxScanf;
extern WCSSTRTOK        _cdeCoreWcsStrTok;
extern WCSSTRPBRKSPN    _cdeWcsStrPbrkSpn;
extern MEMREALLOC       _cdeCoreMemRealloc;
extern MEMSTRXCPY       _cdeMemStrxCpy;
extern MEMSTRXCMP       _cdeMemStrxCmp;
extern OSIFGETTIME      _osifWinNTGetTime;
extern OSIFGETTSCPERSEC _osifWinNTGetTscPerSec;
extern OSIFGETTSC       _osifWinNTGetTsc;
extern OSIFMEMALLOC     _osifWinNTMemAlloc;         /*pMemAlloc     */
extern OSIFMEMFREE      _osifWinNTMemFree;          /*pfREEPages    */
extern OSIFFWRITE       _osifWinNTFileWrite;        /*pFwrite       */
extern OSIFFREAD        _osifWinNTFileRead;         /*pFread        */
extern OSIFFSETPOS      _osifWinNTFileSetPos;       /*pFsetpos      */
extern OSIFFOPEN        _osifWinNTFileOpen;         /*pFopen        */
extern OSIFFCLOSE       _osifWinNTFileClose;        /*pFclose       */
extern OSIFFDELETE      _osifWinNTFileDelete;       /*pFdelete      */
extern OSIFFRENAME      _osifWinNTFileRename;       /*pFrename;     */
extern OSIFFFINDALL     _osifWinNTFileFindAll;      /*pFfindall     */
extern OSIFFGETSTATUS   _osifWinNTFileGetStatus;    /*pFgetstatus   */
extern OSIFDIRCREATE    _osifWinNTDirectoryCreate;  /*pDirCreate    */
extern OSIFCMDEXEC      _osifWinNTCmdExec;          /*pCmdExec      */
extern OSIFGETENV       _osifWinNTGetEnv;           /*pGetEnv           */
extern OSIFGETDCWD      _osifWinNTGetDrvCwd;        /*pGetDrvCwd    current working directory   */

extern COREFILERW       _cdeCoreFread;              /*pFReadCORE    */
extern COREFILERW       _cdeCoreFwrite;             /*pFWriteCORE   */
extern CORESETPOS       _cdeCoreFsetpos;            /*pFSetPosCORE  */
extern COREFFLUSH       _cdeCoreFflush;             /*pFFlushCORE   */

extern int main(int argc, char** argv);
extern int _cdeStr2Argcv(char** argv, char* szCmdline);
extern void __cde80387FINIT(void);

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

extern void _cdeSigDflt(int sig);
extern struct _CDE_LCONV_LANGUAGE _cdeCLocale;
extern struct lconv _cdeLconv_C;

extern char _gCdeStrLibcVersion[];

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

static int _StdInGetChar(void** ppDest) {
    return fgetc((FILE*)CDE_STDIN);
}

static CDE_APP_IF gCdeAppIfWinNT;

//
// CTRL-C support
//
static char fCtrl_C = 0;

static BOOL WINAPI WinNTCtrlCHandlerRoutine(_In_ DWORD dwCtrlType) {

    fCtrl_C |= CTRL_C_EVENT == dwCtrlType;

    return CTRL_C_EVENT == dwCtrlType;
}

void __CdeChkCtrlC(void) {

    if (1 == fCtrl_C) {

        fCtrl_C = 0;
        raise(SIGINT);

    }
}

void* __cdeGetAppIf(void) {
    __CdeChkCtrlC();
    return &gCdeAppIfWinNT;
}

static CDE_SERVICES gCdeServicesWinNT = {/*CDE_PROTOCOL*/
    .TimeAtSystemStart = 0,         /* assuming 01.01.1970  */
    .TSClocksPerSec = 2000000000,   /* assuming 2GHz        */

    .wVerMajor = 0,
    .wVerMinor = 0,
    .fx64Opcode = 8 == sizeof(void*),
    .HeapStart = {(void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1},
    .TSClocksAtCdeTrace = 0,
    .TimeAtSystemStart = 0,
    .ReportStatusCode = { 0 },
    ////    FNDECL_MAINSTART(*pmainstart);          // the fjMainDxeEntryPoint/fjMainSmmEntryPoint loader     /* kg0705F0*/
    ////    FNDECL_SMMSTART(*psmmstart);            // wrapper for fjLoadDriverToSmm
    .pGetConIn = _StdInGetChar,
    .pPutConOut = _StdOutPutChar,
    .pPutDebug = _StdOutPutChar,
    .pVwxPrintf = _cdeCoreVwxPrintf,
    .pVwxScanf = _cdeCoreVwxScanf,
    .pMemRealloc = _cdeCoreMemRealloc,
    .pMemStrxCpy = _cdeMemStrxCpy,
    .pMemStrxCmp = _cdeMemStrxCmp,
    ////// ----- string processing functions
    .pWcsStrPbrkSpn = _cdeWcsStrPbrkSpn,
    .pWcsStrTok = _cdeCoreWcsStrTok,
    //
    // ----- core C functions, running on driver side
    //
        .pFReadCORE = _cdeCoreFread,            /* core fread()     */
        .pFWriteCORE = _cdeCoreFwrite,          /* core fwrite()    */
        .pFSetPosCORE = _cdeCoreFsetpos,        /* core fsetpos()   */
        .pFFlushCORE = _cdeCoreFflush,          /* core fflush()    */

        //
        // OSIF - operating system interface
        //
            .pGetTime = _osifWinNTGetTime,
            .pSetRtcTime = 0,
            .pGetTscPerSec = _osifWinNTGetTscPerSec,
            .pGetTsc = _osifWinNTGetTsc,
            .pMemAlloc = _osifWinNTMemAlloc,
            .pMemFree = _osifWinNTMemFree,
            .pFopen = _osifWinNTFileOpen,            /* OSIFFOPEN        *pFopen;    */
            .pFclose = _osifWinNTFileClose,          /* OSIFFCLOSE       *pFclose;   */
            .pFread = _osifWinNTFileRead,            /* OSIFFREAD        *pFread;    */
            .pFwrite = _osifWinNTFileWrite,          /* OSIFFWRITE       *pFwrite;   */
            .pFsetpos = _osifWinNTFileSetPos,        /* OSIFFSETPOS      *pFsetpos;  */
            .pFdelete = _osifWinNTFileDelete,        /* OSIFFDELETE      *pFdelete;  */
            .pFrename = _osifWinNTFileRename,        /* OSIFFRENAME      *pFrename;..*/
            .pCmdExec = _osifWinNTCmdExec,           /* OSIFCMDEXEC      *pCmdExec;  */
            .pGetEnv = _osifWinNTGetEnv,             /* OSIFGETENV       *pGetEnv;   */
            .pFfindall = _osifWinNTFileFindAll,      /* OSIFFFINDALL     *pFfindall  */
            .pFgetstatus = _osifWinNTFileGetStatus,  /* OSIFFGETSTATUS   *pFgetstatus*/
            .pGetDrvCwd = _osifWinNTGetDrvCwd,       /* OSIFGETDCWD      *pGetDrvCwd */
            .pDirCreate = _osifWinNTDirectoryCreate, /* OSIFDIRCREATE    *pDirCreate */
            .pDirRemove = NULL,                     /* OSIFDIRCREATE    *pDirCreate */
};

static CDE_APP_IF gCdeAppIfWinNT = {
    //.qwMagic0 = 0x53323342494C4324,
    //.qwMagic1 = 0x3E3E3E3E74726174,

    .nErrno = 0,
    .nEfiErrno = 0,

    .pCdeServices = &gCdeServicesWinNT,
    {
        .CommParm.OSIf = WINNTIF,
        .CommParm.CommHandle = 0,
        .CommParm.pCommAPI = 0
    },
    .pStrtokStaticInternal = 0,
    .nNextRandStaticInternal = 0,
    .exit_buf = {0},
    .exit_status = 0/*EFI_SUCCESS*/,
    .rgcbAtexit = {0},
    .lTmpNamNum = 0,
    .rgfnSignal = {_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt,_cdeSigDflt},
    .pIob = &_iob[0],
    .cIob = CDE_FILEV_MAX,
    .bmRuntimeFlags = TIANOCOREDEBUG,
    .ActiveLocale = { "C", &_cdeLconv_C, NULL }/*&_cdeCLocale*/
};

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
int _MainEntryPointWinNT(void)
{
    int Status = 0;

    __cdeChkStkAddr = __chkstkWindows;

    do {
        int i;
        int argc;
        CDEFILE* pCdeFile = (CDEFILE*)&_iob[0];
        void* argvex[CDE_ARGV_MAX + 2] = { NULL,NULL };//ADD SUPPORT FOR argv[-1] argv[-2]
        CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();

        //
        // redirect CTRL-C handler from OS to App
        //
        SetConsoleCtrlHandler(WinNTCtrlCHandlerRoutine, 1);

        memset(&_iob[0], 0, sizeof(_iob));                          // clear entire structure

        /*stdin->*/ pCdeFile[0].fRsv = TRUE;                        // reserved/in use
        /*stdin->*/ pCdeFile[0].emufp = /* -=PORTING=- */(void*)GetStdHandle(STD_INPUT_HANDLE);
        /*stdin->*/ pCdeFile[0].openmode = O_TEXT + O_RDONLY + O_CDENOSEEK + O_CDESTDIN + (FILE_TYPE_DISK == GetFileType(pCdeFile[0].emufp) ? O_CDEREDIR : 0);

        /*stdout->*/pCdeFile[1].fRsv = TRUE;                        // reserved/in use
        /*stdout->*/pCdeFile[1].emufp = /* -=PORTING=- */(void*)GetStdHandle(STD_OUTPUT_HANDLE);
        /*stdout->*/pCdeFile[1].openmode = O_TEXT + O_WRONLY + O_CDENOSEEK + O_CDESTDOUT + (FILE_TYPE_DISK == GetFileType(pCdeFile[1].emufp) ? O_CDEREDIR : 0);

        /*stderr->*/pCdeFile[2].fRsv = TRUE;                        // reserved/in use
        /*stderr->*/pCdeFile[2].emufp = /* -=PORTING=- */(void*)GetStdHandle(STD_ERROR_HANDLE);
        /*stderr->*/pCdeFile[2].openmode = O_TEXT + O_WRONLY + O_CDENOSEEK + O_CDESTDERR + (FILE_TYPE_DISK == GetFileType(pCdeFile[2].emufp) ? O_CDEREDIR : 0);

        //
        // FPU Coprocessor 80387 initialization
        // 
        __cde80387FINIT();

        //
        // clock() initialization
        //
        /* -=PORTING=- */ pCdeAppIf->pCdeServices->TSClocksPerSec = gCdeServicesWinNT.pGetTscPerSec(pCdeAppIf, 0);
        pCdeAppIf->pCdeServices->TimeAtSystemStart = gCdeServicesWinNT.pGetTime(pCdeAppIf);
        pCdeAppIf->pCdeServices->TSClocksAtSystemStart = gCdeServicesWinNT.pGetTsc(pCdeAppIf);
        //
        // split up the Windows NT command line
        //
        argc = _cdeStr2Argcv((char**)&argvex[0 + 2], GetCommandLineA());

        gszCdeDriverName = argvex[0 + 2];
        //
        // setup the "atexit" registration buffer (N1256 chap. 7.20.4.2)
        // http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=327
        //
                //a:\edk2-master20160712.r71\CdePkg\CdeOSIF\WinNT\osifWinNTEntryPoint.c(217) : warning C4054: 'type cast' : from function pointer 'void (__cdecl *)(void)' to data pointer 'void *'
        memset((void*)&pCdeAppIf->rgcbAtexit[0], 0, sizeof(pCdeAppIf->rgcbAtexit));

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

        //
        // version string
        //
        if (2 == argc && 0 == strcmp(argvex[1 + 2], "--TOROCVER"))
        {
            fprintf(stdout, "%s\n", _gCdeStrLibcVersion);
            return 3;
        }
        //
        // invoke "main()"
        //
        Status = setjmp(pCdeAppIf->exit_buf) ? pCdeAppIf->exit_status : main(argc, (char**)&argvex[2]);

        //
        // run the "atexit" registered functions (N1124 chap. 7.20.4.2)
        //
        for (i = CDE_ATEXIT_REGISTRATION_NUM - 1; i >= 0; i--) {
            if (NULL != pCdeAppIf->rgcbAtexit[i]) {
                (*pCdeAppIf->rgcbAtexit[i])();
            }
        }
        //
        // close open files
        //
        fflush(/*stdout*/(FILE*)CDE_STDOUT);    // NULL,EOF,0,stream == flush parameter
        fflush(/*stderr*/(FILE*)CDE_STDERR);    // NULL,EOF,0,stream == flush parameter
        for (i = 3/*skip stdin,stdout,stderr*/; i < CDE_FILEV_MAX; i++)
            fclose((FILE*)&pCdeFile[i]);

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

    } while (_0);

    return Status;
}

/** _cdeCRT0UefiShell() - CRT0 for UEFI Shell in a non-EDK build environment

Synopsis
    #include <CdeServices.h>
    int _cdeCRT0WinNT(void);
Description
    C runtime initialization.
Returns
    Status
**/
int _cdeCRT0WinNT(void)
{
    return _MainEntryPointWinNT();
}
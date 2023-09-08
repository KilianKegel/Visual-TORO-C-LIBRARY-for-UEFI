//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage _MainEntryPointLinux

\brief Implementation of the <em>_MainEntryPointLinux</em> function

\file osifLinuxEntryPoint.c

*/
//#define OS_EFI
#include <cde.h>
#include <CdeServices.h>
//
// stdio.h
//
extern __declspec(dllimport) size_t fwrite(const void* ptr, size_t size, size_t nelem, FILE* stream);
extern __declspec(dllimport) int fgetc(FILE* stream);
extern __declspec(dllimport) int fclose(FILE* stream);

#define EOF    (-1)
//
// signal.h
//
#define SIGINT  2
extern __declspec(dllimport) int raise(int sig);

//
// setjmp.h
//

CDEFILE _iob[CDE_FILEV_MAX];                                  /* Microsoft definition. Since it must be buildable within the DDK*/
char* gszCdeDriverName;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);

//
// externs
//
extern void free(void*);

extern _PUTCHAR  _CdeDbgPutChar;
extern VWXPRINTF VwxPrintf;
extern VWXSCANF VwxScanf;
extern WCSSTRTOK WcsStrTok;
extern WCSSTRPBRKSPN WcsStrPbrkSpn;
extern MEMREALLOC MemRealloc;
extern MEMSTRXCPY MemStrxCpy;
extern MEMSTRXCMP MemStrxCmp;
extern OSIFGETTIME      _osifLinuxGetTime;
extern OSIFGETTSCPERSEC _osifLinuxGetTscPerSec;
extern OSIFGETTSC       _osifLinuxGetTsc;
extern OSIFMEMALLOC     _osifLinuxMemAlloc;      /*posifMemAlloc*/
extern OSIFMEMFREE      _osifLinuxMemFree;       /*pfREEPages*/
extern OSIFFWRITE       _osifLinuxFileWrite;     /*posifFwrite*/
extern OSIFFREAD        _osifLinuxFileRead;      /*posifFread*/
extern OSIFFSETPOS      _osifLinuxFileSetPos;    /*posifFsetpos*/
extern OSIFFOPEN        _osifLinuxFileOpen;      /*posifFopen*/
extern OSIFFCLOSE       _osifLinuxFileClose;     /*posifFclose*/
extern OSIFFDELETE      _osifLinuxFileDelete;    /*posifFdelete*/
extern OSIFFRENAME      _osifLinuxFileRename;    /*posifFrename;..*/
//TODO extern OSIFFFINDALL     _osifUefiShellFileFindAll;       /*pFfindall     */
//TODO extern OSIFFGETSTATUS   _osifUefiShellFileGetStatus;     /*pFgetstatus   */
//TODO extern OSIFDIRCREATE    _osifUefiShellDirectoryCreate;   /*pDirCreate    */
extern OSIFCMDEXEC      _osifLinuxCmdExec;       /*posifCmdExec*/
extern OSIFGETENV       _osifLinuxGetEnv;        /*posifGetEnv   */
//TODO extern OSIFGETDCWD      _osifUefiShellGetDrvCwd;         /*pGetDrvCwd    current working directory*/

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
extern int main(int argc, char** argv);
extern char _gCdeCopyRight[2];
extern int _CdeStr2Argv(char** argv, char* szCmdline);
extern struct lconv _cdeLconv_C;
extern void _cdeSigDflt(int sig);
extern struct LCONV_LANGUAGE_TAG _locale_C_;

static void _StdOutPutChar(int c, void** ppDest) {

    //TODO
    //    _syscall(/* sys_write */ 1, (int64_t)1, (int64_t)&c, (int64_t)1, 0, 0, 0);

    //return;
    
    if (c == EOF)
        fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);
    else
        fwrite(&c, 1, 1, (FILE*)CDE_STDOUT);
    fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);
}

static int _StdInGetChar(void** ppDest) {
    return fgetc((FILE*)CDE_STDIN);
}

CDE_APP_IF gCdeAppIfLinux;

//
// CTRL-C support
//
extern void __restore_rt(void);
static volatile char fCtrl_C = 0;

static void LinuxCtrlCHandlerRoutine(int signum)
{
    fCtrl_C |= 1;
   
}

void __CdeChkCtrlC(void) {

    if (1 == fCtrl_C) {

        fCtrl_C = 0;

        raise(SIGINT);

    }
}

CDE_APP_IF* _GetCdeAppIf(void) {
    __CdeChkCtrlC();
    return &gCdeAppIfLinux;
}

CDE_SERVICES gCdeServicesLinux = {/*CDE_PROTOCOL*/
    0x55,//    UINT16 wVerMajor;
    0xaa,//    UINT16 wVerMinor;
    //////todo filedate??? __TIMESTAMP__
    ////    UINT16 wPtclSize;
    .fx64Opcode = 8 == sizeof(void*),
    .HeapStart = {(void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1},
    .TSClocksAtSystemStart = 0,
    .TSClocksAtCdeTrace = 0,
    .TSClocksPerSec = 0,
    .TimeAtSystemStart = 0,
    .ReportStatusCode = 0,
    ////    FNDECL_MAINSTART(*pmainstart);          // the fjMainDxeEntryPoint/fjMainSmmEntryPoint loader     /* kg0705F0*/
    ////    FNDECL_SMMSTART(*psmmstart);            // wrapper for fjLoadDriverToSmm
    .pGetConIn = _StdInGetChar,
    .pPutConOut = _StdOutPutChar,
    .pPutDebug = _StdOutPutChar,
    .pVwxPrintf = VwxPrintf,
    .pVwxScanf = VwxScanf,
    .pMemRealloc = MemRealloc,
    .pMemStrxCpy = MemStrxCpy,
    .pMemStrxCmp = MemStrxCmp,
    ////// ----- string processing functions
    .pWcsStrPbrkSpn = WcsStrPbrkSpn,
    .pWcsStrTok = WcsStrTok,
    //
    // OSIF - operating system interface
    //
        .pGetTime = _osifLinuxGetTime,
        .pSetRtcTime = 0,
        .pGetTscPerSec = _osifLinuxGetTscPerSec,
        .pGetTsc = _osifLinuxGetTsc,
        .pMemAlloc = _osifLinuxMemAlloc,
        .pMemFree = _osifLinuxMemFree,
        .pFopen = _osifLinuxFileOpen,       /* OSIFFOPEN        *posifFopen;    */
        .pFclose = _osifLinuxFileClose,     /* OSIFFCLOSE       *posifFclose;   */
        .pFread = _osifLinuxFileRead,       /* OSIFFREAD        *posifFread;    */
        .pFwrite = _osifLinuxFileWrite,     /* OSIFFWRITE       *posifFwrite;   */
        .pFsetpos = _osifLinuxFileSetPos,   /* OSIFFSETPOS      *posifFsetpos;  */
        .pFdelete = _osifLinuxFileDelete,   /* OSIFFDELETE      *posifFdelete;  */
        .pFrename = _osifLinuxFileRename,   /* OSIFFRENAME      *posifFrename;..*/
        .pCmdExec = _osifLinuxCmdExec,      /* OSIFCMDEXEC      *posifCmdExec;  */
        .pGetEnv = _osifLinuxGetEnv,        /* OSIFGETENV       *posifGetEnv;   */
        .pFfindall = NULL,                  /* OSIFFFINDALL     *pFfindall      */
        .pFgetstatus = NULL,                /* OSIFFGETSTATUS   *pFgetstatus    */
        .pGetDrvCwd = NULL,                 /* OSIFGETDCWD      *pGetDrvCwd     */
        .pDirCreate = NULL,                 /* OSIFDIRCREATE    *pDirCreate     */
        .pDirRemove = NULL,                 /* OSIFDIRCREATE    *pDirCreate     */
    //
    // diagnostic
    //
};

CDE_APP_IF CdeAppIfLinux = {

    .nErrno = 0,
    .nEfiErrno = 0,
    
    .pCdeServices = &gCdeServicesLinux,
    {
        .CommParm.OSIf = LINUXIF,
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
    .bmRuntimeFlags = 0,    /* clear all runtimeflags   */
    .ActiveLocale = { "C", &_cdeLconv_C, NULL }/*&_cdeCLocale*/
};


int _osifLinuxEntryPointIntern(void** rsp)
{
    int Status = 0;
    intptr_t argc = (intptr_t) (rsp[0]);
    char** argv = &((char*)(rsp[1]));
    char** envp = argv;
    while (*envp++)
        ;
    //////__debugbreak();
    if (_gCdeCopyRight[0])
    {
        do {
            int i;
            CDEFILE* pCdeFile = (CDEFILE*)&_iob[0];
            //void* argvex[CDE_ARGV_MAX + 2] = { NULL,NULL };//ADD SUPPORT FOR argv[-1] argv[-2]
            CDE_APP_IF* pCdeAppIf = _GetCdeAppIf();

            pCdeAppIf->pIob = pCdeFile;

            //memset(pCdeFile, 0, sizeof(_iob));                        // clear entire structure

            /*stdin->*/ pCdeFile[0].fRsv = TRUE;								                                                                        // resered/in use
            /*stdin->*/ pCdeFile[0].emufp = /* -=PORTING=- */(void*)0;
            /*stdin->*/ pCdeFile[0].openmode = O_TEXT + O_RDONLY + O_CDENOSEEK + O_CDESTDIN /*TODO + (FILE_TYPE_DISK == GetFileType(pCdeFile[0].emufp) ? O_CDEREDIR : 0)*/;

            /*stdout->*/pCdeFile[1].fRsv = TRUE;								// resered/in use
            /*stdout->*/pCdeFile[1].emufp = /* -=PORTING=- */(void*)1;
            /*stdout->*/pCdeFile[1].openmode = O_TEXT + O_WRONLY + O_CDENOSEEK + O_CDESTDOUT /*TODO + (FILE_TYPE_DISK == GetFileType(pCdeFile[1].emufp) ? O_CDEREDIR : 0)*/;

            /*stderr->*/pCdeFile[2].fRsv = TRUE;								// resered/in use
            /*stderr->*/pCdeFile[2].emufp = /* -=PORTING=- */(void*)2;
            /*stderr->*/pCdeFile[2].openmode = O_TEXT + O_WRONLY + O_CDENOSEEK + O_CDESTDERR /*TODO + (FILE_TYPE_DISK == GetFileType(pCdeFile[2].emufp) ? O_CDEREDIR : 0)*/;

            ////__debugbreak();
            ////__debugbreak();
            //
            // clock() initialization
            //
            /* -=PORTING=- */ pCdeAppIf->pCdeServices->TSClocksPerSec = gCdeServicesLinux.pGetTscPerSec(pCdeAppIf);
            pCdeAppIf->pCdeServices->TimeAtSystemStart = gCdeServicesLinux.pGetTime(pCdeAppIf);
            pCdeAppIf->pCdeServices->TSClocksAtSystemStart = gCdeServicesLinux.pGetTsc(pCdeAppIf);
            
            //
            // redirect CTRL-C handler from OS to App
            //
            if (1)
            {
                size_t qwRet = (size_t)-1LL;
                /* Type of a signal handler.  */
                typedef void (__cdecl *_crt_signal_t) (int);
                struct /*kernel_*/sigact/*ion*/
                {
                    _crt_signal_t handler;
                    uint64_t flags;
                    void (*restorer) (void);
                    uint64_t mask;
                }_sigact = {
                    .handler = LinuxCtrlCHandlerRoutine,
                    .flags = 0x04000000,
                    .restorer = __restore_rt,
                    .mask = 0
                };
                qwRet = _syscall(13/*sys_rt_sigaction*/, SIGINT, (int64_t)&_sigact, (int64_t)NULL, 8, 0, 0);
            }

            //gszCdeDriverName = argvex[0 + 2];
            gszCdeDriverName = argv[0];

            ////__debugbreak();
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
            // invoke "main()"
            //
            Status = setjmp(pCdeAppIf->exit_buf) ? pCdeAppIf->exit_status : main((int)argc, (char**)argv);

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
            for (i = 3/*skip stdin,stdout,stderr*/; i < CDE_FILEV_MAX; i++)
                fclose((FILE*)&_iob[i]);

            //
            // flush and close stdout + stderr, if redirected
            //
            fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDOUT);    // NULL,EOF,0,stream == flush parameter
            fwrite(NULL, (size_t)EOF, 0, (FILE*)CDE_STDERR);    // NULL,EOF,0,stream == flush parameter

            for (i = 1/*skip stdin*/; i <= 2; i++)
                if (O_CDEREOPEN & _iob[i].openmode)
                    fclose((FILE*)&_iob[i]);

            //
            // free memory allocated during runtime
            //
            // automatically freed with sys_exit

            _syscall(60/*sys_exit*/, Status, 0, 0, 0, 0, 0);


        } while (0);
    }
        return Status;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _osifWinNTCmdExec.c

Abstract:

    OS interface (osif) to run a command on system shell

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <cde.h>
#define ALIENOS

#include <CdeServices.h>
//#ifdef NMOFINE
//    #undef NMOFINE
//#endif//NMOFINE

#define UINT64 unsigned long long
#define UINT8 unsigned char
#define CDE_APP_IF void

/** _osifCmdExec() - run command on system shell

Synopsis
    #include <CdeServices.h>
    int _osifWinNTCmdExec(CDE_APP_IF* pCdeAppIf, char* szCommand);
Description
    Run command on system shell
Parameters
    CDE_APP_IF* pCdeAppIf   - application interface
    char* szCommand         - command to run
Returns
    return value of the command
**/
int _osifWinNTCmdExec(void* pCdeAppIf, char* szCommand) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char szShell[CDE_COMSPEC_LEN];
    char szSlashCExtendetCommand[CDE_SLASHCEXTENDETCOMMAND_LEN] = { "/c " };
    /* /c      Carries out the command specified by string and then terminates */
    unsigned  nSize, u;
    DWORD exit_code = (DWORD)-1;
    int nRet = 1;
    //    int nSize = -1;
    //
    do {

        // ----- detect verify and validate COMSPEC environment variable

        nSize = GetEnvironmentVariable("ComSpec", &szShell[0], sizeof(szShell));

        if (NULL == szCommand) {

            nRet = nSize != 0;
            /* NOTE: the COMSPEC variable is just presence checked, original Windows does more... */
            break;

        }

        if (sizeof(szSlashCExtendetCommand) < (strlen(szCommand) + sizeof("/c "))) {
            nRet = 0;
            break;
        }
        //        strcpy(szSlashCExtendetCommand, szShell);
        //        strcpy(&szSlashCExtendetCommand[strlen(szSlashCExtendetCommand)], " /c ");
        strcpy(szSlashCExtendetCommand, "/c ");
        strcpy(&szSlashCExtendetCommand[strlen(szSlashCExtendetCommand)], szCommand);

        memset(&si, 0, sizeof(si));
        memset(&pi, 0, sizeof(pi));

        si.cb = sizeof(si);

        //        if(!CreateProcessA(szShell,szSlashCExtendetCommand,NULL,NULL,0,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi)){
        if (!CreateProcessA(szShell, szSlashCExtendetCommand, NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
            nRet = 1;
            break;
        }

        // Wait until child process exits.
        WaitForSingleObject(pi.hProcess, INFINITE);

        u = GetExitCodeProcess(pi.hProcess, &exit_code);

        //printf("the execution of: \"%s\"\nreturns: %d\n", commandLine, exit_code);
        // Close process and thread handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        nRet = exit_code;

    } while (0);

    //    memset(&szShell,0,sizeof(szShell));
    //    printf("Welcome, to the jungle...\n");
    //    printf("%x\n",system(NULL));
    //    system("echo hello world");
    //    GetEnvironmentVariable("ComSpec",&szShell[0],sizeof(szShell));
    //    printf("%s\n",szShell);
    //    exit(3);
    //    //printf("%s\n",getenv("ComSpec"));
    //    //return 0;
    //    memset(&si,0,sizeof(si));
    //    si.cb = sizeof(si);
    //    memset(&pi,0,sizeof(pi));
    //    if(!CreateProcessA(szShell,"/c pause",NULL,NULL,0,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi))
    //        printf("failed...\n");
    //
    return nRet;
}


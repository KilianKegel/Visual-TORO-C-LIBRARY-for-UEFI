//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxCmdExec

\brief Implementation of the <em>osifUefiDxeEntryPoint</em> function

\file osifLinuxCmdExec.c

*/
#include <stdio.h>
#include <string.h>
#include <cde.h>
#include <CdeServices.h>
//
// prototype
//
OSIFCMDEXEC osifLinuxCmdExec;

#pragma warning( disable : 4101 )

int32_t osifLinuxCmdExec(IN CDE_APP_IF* pCdeAppIf, const char* pstrCommand) 
{
    //STARTUPINFO si;
    //PROCESS_INFORMATION pi;
    //char szShell[CDE_COMSPEC_LEN];
    //char szSlashCExtendetCommand[CDE_SLASHCEXTENDETCOMMAND_LEN] = { "/c " };
    ///* /c      Carries out the command specified by string and then terminates */
    //unsigned  nSize, u;
    //DWORD exit_code = (DWORD)-1;
    int nRet = 1;
    ////    int nSize = -1;
    ////
    //CDEMOFINE((MFNINF(1) "-->\n"));

    //do {

    //    // ----- detect verify and validate COMSPEC environment variable

    //    nSize = GetEnvironmentVariable("ComSpec", &szShell[0], sizeof(szShell));

    //    if (NULL == pstrCommand) {

    //        nRet = nSize != 0;
    //        /* NOTE: the COMSPEC variable is just presence checked, original Windows does more... */
    //        break;

    //    }

    //    if (sizeof(szSlashCExtendetCommand) < (strlen(pstrCommand) + sizeof("/c "))) {
    //        nRet = 0;
    //        break;
    //    }
    //    //        strcpy(szSlashCExtendetCommand, szShell);
    //    //        strcpy(&szSlashCExtendetCommand[strlen(szSlashCExtendetCommand)], " /c ");
    //    strcpy(szSlashCExtendetCommand, "/c ");
    //    strcpy(&szSlashCExtendetCommand[strlen(szSlashCExtendetCommand)], pstrCommand);

    //    CDEMOFINE((MFNINF(1) "%s\n", szSlashCExtendetCommand));

    //    memset(&si, 0, sizeof(si));
    //    memset(&pi, 0, sizeof(pi));

    //    si.cb = sizeof(si);

    //    //        if(!CreateProcessA(szShell,szSlashCExtendetCommand,NULL,NULL,0,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi)){
    //    if (!CreateProcessA(szShell, szSlashCExtendetCommand, NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
    //        CDEMOFINE((MFNERR(1) "Can not run \"%s\"\n", pstrCommand));
    //        nRet = 1;
    //        break;
    //    }

    //    // Wait until child process exits.
    //    WaitForSingleObject(pi.hProcess, INFINITE);

    //    u = GetExitCodeProcess(pi.hProcess, &exit_code);

    //    CDEMOFINE((MFNINF(1) "\"%s\" returned ERRORCODE %d, u == %d\n", pstrCommand, exit_code, u));

    //    //printf("the execution of: \"%s\"\nreturns: %d\n", commandLine, exit_code);
    //    // Close process and thread handles.
    //    CloseHandle(pi.hProcess);
    //    CloseHandle(pi.hThread);

    //    nRet = exit_code;

    //} while (0);

    return nRet;
}


/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    PostBuild.c

Abstract:

    toro C Library build tool

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

char cmdline1[512];
char cmdline2[512];
char line[512];

char* eos(char* str)
{
    return &str[strlen(str)];
}
//
//              0                       1               2               3                   4             5                 6
// 
//cmd / C tools\PostBuildEvent.bat $(SolutionDir) $(ProjectDir) $(Configuration) $(PlatformArchitecture) $(IntDir) $(SolutionDir)lib\toroC$(PlatformArchitecture).lib
//
int main(int argc, char** argv) {
    time_t timer = time(NULL);
    struct tm* time = gmtime(&timer);
    char str3264[sizeof("64")], *pstr3264 = strcpy(str3264,argv[4]);    // get build platform 64 vs.32 $(PlatformArchitecture)

    printf("-->> str3264 %s\n", str3264);

    if (1) 
    {
        int i;
        for (i = 0; i < argc; i++)
            printf("argv[%d]: %s\n", i, argv[i]);
    }

    do {
        //
        // get .OBJ list
        //
        //sprintf(cmdline1, "lib /nologo /list %sCdePkg\\CdeLib\\%s\\CdeLib.lib > %s\\obj.lst", argv[1],0 == strcmp(argv[4],"x64") ? "x64" : "ia32" ,argv[4]);
        sprintf(cmdline1, "lib /nologo /list %s | find \"CDEABI\" > %sCDEABI%s.lst",
            argv[6]/*$(SolutionDir)lib\toroC$(PlatformArchitecture).lib*/,
            argv[5],/*$(IntDir)*/
            str3264);
                                                                        

        printf("%s\n", cmdline1);
        system(cmdline1);

        sprintf(cmdline1, "lib /nologo /list %s | find \"\\osif\" > %sOSIF%s.lst",
            argv[6]/*$(SolutionDir)lib\toroC$(PlatformArchitecture).lib*/,
            argv[5],/*$(IntDir)*/
            str3264);


        printf("%s\n", cmdline1);
        system(cmdline1);

        //
        // create Microsoft Linker command file to remove all NON-CDEABI (including entrypoint .OBJ) CdePkg\CdeLib\removeNONCDEABI64/32MSFT.lst
        //
        if (1)
        {
            static char strPathFileOUT[256];
            static char strPathFileIN[256];
            FILE* fpCDEABIOUT_1;
            FILE* fpCDEABIIN;
            char strLineBuf[128], * pLine = NULL;

            //
            // generate IN/OUT filenames for OSIF removal
            //
            sprintf(strPathFileOUT, "%s..\\CdePkg\\%s\\removeNONCDEABI%sMSFT.lst",
                argv[2],/*$(ProjectDir)*/
                argv[3],/*$(Configuration)*/
                str3264);

            sprintf(strPathFileIN, "%sCDEABI%s.lst",
                argv[5],/*$(IntDir)*/
                str3264);

            printf("%s\n", strPathFileOUT);
            printf("%s\n", strPathFileIN);

            if (NULL == (fpCDEABIOUT_1 = fopen(strPathFileOUT, "w")))
                fprintf(stderr, "FATAL: FAILED TO OPEN \"%s\"\n", strPathFileOUT),
                exit(EXIT_FAILURE);

            if (NULL == (fpCDEABIIN = fopen(strPathFileIN, "r")))
                fprintf(stderr, "FATAL: FAILED TO OPEN \"%s\"\n", strPathFileIN),
                exit(EXIT_FAILURE);

            while (NULL != (pLine = fgets(strLineBuf, sizeof(strLineBuf), fpCDEABIIN)))
                strcpy(strstr(pLine,"CDEABI"),""),  /* cut at CDEABI.obj */
                /*printf("%s", pLine),*/
                fprintf(fpCDEABIOUT_1, "/REMOVE:%s.obj\n", pLine);
        }
        
        //
        // create Microsoft Linker command file to remove all OSIF (including entrypoint .OBJ) CdePkg\CdeLib\removeOSIF64/32MSFT.lst
        //
        if (1)
        {
            static char strPathFileOUT[256];
            static char strPathFileIN[256];
            FILE* fpOUT;
            FILE* fpIN;
            char strLineBuf[128], * pLine = NULL;

            //
            // generate IN/OUT filenames for OSIF removal
            //
            sprintf(strPathFileOUT, "%s..\\CdePkg\\%s\\removeOSIF%sMSFT.lst",
                argv[2],/*$(ProjectDir)*/
                argv[3],/*$(Configuration)*/
                str3264);

            sprintf(strPathFileIN, "%sOSIF%s.lst",
                argv[5],/*$(IntDir)*/
                str3264);

            printf("%s\n", strPathFileOUT);
            printf("%s\n", strPathFileIN);

            if (NULL == (fpOUT = fopen(strPathFileOUT, "w")))
                fprintf(stderr, "FATAL: FAILED TO OPEN \"%s\"\n", strPathFileOUT),
                exit(EXIT_FAILURE);

            if (NULL == (fpIN = fopen(strPathFileIN, "r")))
                fprintf(stderr, "FATAL: FAILED TO OPEN \"%s\"\n", strPathFileIN),
                exit(EXIT_FAILURE);

            while (NULL != (pLine = fgets(strLineBuf, sizeof(strLineBuf), fpIN)))
                fprintf(fpOUT, NULL != strstr(pLine, "\\osif") ? "/REMOVE:%s" : "", pLine);
        }
        
        //
        // Create LLVM linker enabled libraries by removing certain .OBJ modules.
        // 
        // NOTE: The LLVM linker cannot handle libraries containing multiple .OBJ modules exporting the same SYMBOL.
        //       Unlike the Microsoft linker, the LLVM linker does not follow SYMBOL SEQUENCE TRACE, beginning at the /ENTRY:"CRT0_NAME".
        //       For this reason, the LLVM linker does not support multi-entrypoint libraries like the TORO C Library.
        //       As a workaround, remove all .OBJ modules from the library that are not required for a particular target operating system.
        //       For example:
        //       - For WINNT TARGET, remove all UefiShell-, UefiDxe-, and UefiPei-related .OBJ modules.
        //       - For UEFISHELL TARGET, remove all WinNT-, UefiDxe-, and UefiPei-related .OBJ modules.
        if (1)
        {
            char strPathFileOUT[256];
            char strPathFileIN[256];

            sprintf(cmdline1, "lib /nologo /list %s  > %sALLOBJ%s.lst",
                argv[6]/*$(SolutionDir)lib\toroC$(PlatformArchitecture).lib*/,
                argv[5],/*$(IntDir)*/
                str3264);

            printf("%s\n", cmdline1);
            system(cmdline1);

            //
            // Uefi Shell  first
            //
            if (1)
            {
                sprintf(strPathFileIN, "%sALLOBJ%s.lst", argv[5],/*$(IntDir)*/ str3264);
                sprintf(strPathFileOUT, "%sREMOVE4UEFISHELL%s.lst", argv[5],/*$(IntDir)*/ str3264);

                FILE* fpIN = fopen(strPathFileIN, "r");
                FILE* fpOUT = fopen(strPathFileOUT, "w");

                fgets(line, sizeof(line), fpIN); // skip first line

                while (fgets(line, sizeof(line), fpIN))
                {
                    // to lower line
                    for (int i = 0; '\0' != line[i]; i++)
                        line[i] = tolower(line[i]);
                    if (NULL != strstr(line, "osif")) {
                        if (NULL != strstr(line, "winnt") ||
                            NULL != strstr(line, "uefipei") ||
                            NULL != strstr(line, "uefidxe") ||
                            NULL != strstr(line, "uefismm") ||
                            NULL != strstr(line, "uefishelldrv") ||
                            NULL != strstr(line, "entrypointw") ||
                            NULL != strstr(line, "entrypointedk"))
                        {
                            fprintf(fpOUT, "/REMOVE:%s", line);
                        }
                    }

                    if (NULL != strstr(line, "_dfltgetcdeappif"))
                    {
                        fprintf(fpOUT, "/REMOVE:%s", line);
                    }

                }

                fclose(fpIN);
                fclose(fpOUT);

            }

            //
            // WinNT  second
            //
            if (1)
            {
                sprintf(strPathFileIN, "%sALLOBJ%s.lst", argv[5],/*$(IntDir)*/ str3264);
                sprintf(strPathFileOUT, "%sREMOVE4WINNT%s.lst", argv[5],/*$(IntDir)*/ str3264);

                FILE* fpIN = fopen(strPathFileIN, "r");
                FILE* fpOUT = fopen(strPathFileOUT, "w");

                fgets(line, sizeof(line), fpIN); // skip first line

                while (fgets(line, sizeof(line), fpIN))
                {
                    // to lower line
                    for (int i = 0; '\0' != line[i]; i++)
                        line[i] = tolower(line[i]);
                    if (NULL != strstr(line, "osif")) {
                        if (NULL != strstr(line, "uefishell") ||
                            NULL != strstr(line, "uefipei") ||
                            NULL != strstr(line, "uefidxe") ||
                            NULL != strstr(line, "uefismm") ||
                            NULL != strstr(line, "uefishelldrv") ||
                            NULL != strstr(line, "entrypointw") ||
                            NULL != strstr(line, "entrypointedk"))
                        {
                            fprintf(fpOUT, "/REMOVE:%s", line);
                        }
                    }

                    if (NULL != strstr(line, "_dfltgetcdeappif"))
                    {
                        fprintf(fpOUT, "/REMOVE:%s", line);
                    }

                }

                fclose(fpIN);
                fclose(fpOUT);
            }


        }
        
        exit(EXIT_SUCCESS);

    } while (0);

    return 0;
}
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
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
//cmd / C tools\PostBuildEvent.bat $(SolutionDir) $(ProjectDir) $(Configuration) $(PlatformShortName) $(IntDir) $(SolutionDir)lib\toroC$(PlatformArchitecture).lib
//
int main(int argc, char** argv) {
    time_t timer = time(NULL);
    struct tm* time = gmtime(&timer);
    char str3264[sizeof("64")], *pstr3264 = strcpy(str3264,0 == strcmp(argv[4],"x64") ? "64" : "32");// get build platform x64 vs. x86

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
        
        exit(EXIT_SUCCESS);

    } while (0);

    return 0;
}
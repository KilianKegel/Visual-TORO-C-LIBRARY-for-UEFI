/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
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

int main(int argc, char** argv) {
    time_t timer = time(NULL);
    struct tm* time = gmtime(&timer);
    
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
        sprintf(cmdline1, "lib /nologo /list %sCdePkg\\CdeLib\\%s\\CdeLib.lib > %s\\obj.lst", argv[1],0 == strcmp(argv[4],"x64") ? "x64" : "ia32" ,argv[4]);
        printf("%s\n", cmdline1);
        system(cmdline1);

        if (1)
        {
            char filepathname[32];
            int filepathnamelen = sprintf(filepathname, "%s\\obj.lst", argv[4]);
            FILE* fp = fopen(filepathname,"r");

            char* pStr;
            int i = 0;
            
            printf("\n\n%s\n\n", filepathname);
            
            while (NULL != (pStr = fgets(line, sizeof(line), fp)))
            {
                if (NULL != strstr(pStr, "dflt"))
                    continue;
                
                pStr[strlen(pStr) - 1] = '\0';

                //sprintf(cmdline1, "lib /nologo /extract:%s /out:x64\\mod%03X.obj %sCdePkg\\CdeLib\\x64\\CdeLib.lib\n", pStr, i++, argv[1]);
                sprintf(cmdline1, "lib /nologo /extract:%s /out:%s\\mod%03X.obj %sCdePkg\\CdeLib\\%s\\CdeLib.lib\n", pStr, argv[4], i++, argv[1], 0 == strcmp(argv[4], "x64") ? "x64" : "ia32");
                printf("%s", cmdline1);
                system(cmdline1);
            }

            rewind(fp);

            while (NULL != (pStr = fgets(line, sizeof(line), fp)))
            {
                if (NULL == strstr(pStr, "dflt"))
                    continue;

                pStr[strlen(pStr) - 1] = '\0';

                //sprintf(cmdline1, "lib /nologo /extract:%s /out:x64\\mod%03X.obj %sCdePkg\\CdeLib\\x64\\CdeLib.lib\n", pStr, i++, argv[1]);
                sprintf(cmdline1, "lib /nologo /extract:%s /out:%s\\mod%03X.obj %sCdePkg\\CdeLib\\%s\\CdeLib.lib\n", pStr, argv[4], i++, argv[1], 0 == strcmp(argv[4], "x64") ? "x64" : "ia32");
                printf("%s", cmdline1);
                system(cmdline1);
            }
        }

    } while (0);

    return 0;
}
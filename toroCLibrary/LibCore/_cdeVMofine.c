/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeVMofine.c

Abstract:

    CDE internal worker function for all mofine-related functions
    This is a deprecated feature

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>
#include <CDE.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>

extern char _gCdeCfgMofineRawSeparator[];
extern int _cdeCoreSprintf(CDE_APP_IF* pCdeAppIf, char* pszDest, const char* pszFormat, ...);
extern clock_t _cdeCoreClock(CDE_APP_IF* pCdeAppIf);
extern void _cdeCoreExit(CDE_APP_IF* pCdeAppIf, int status);
extern int _cdeCoreMofine(CDE_APP_IF* pCdeAppIf, char* pszDriver, char* pszFile, int nLine, char* pszFunction, char* pszClass, int fTraceEn, char* pszFormat, ...);

int _cdeVMofine(CDE_APP_IF* pCdeAppIf, char* pszDriver, char* pszFile, int nLine, char* pszFunction, char* pszClass, int fTraceEn, char* pszFormat, va_list ap) {
    int nRet = 0;
    char szLine[16];
    ROMPARM_VWXPRINTF RomParm = { \
        /*fForceToDataSeg       */ 1 ,\
        /*fPointerIsParm        */ 0 ,\
        /*fPointerIsFilePointer */ 0 ,\
        /*fCountIsParm;         */ 0 ,\
        /*fAjustDifference      */ 0 ,\
        /*fWide                 */ 0 ,\
        /*fUEFIFormat           */ 0 != (MOFINE_UEFIFMTSTR & fTraceEn),\
    };

    if ((fTraceEn & 1) == 1) {

        char* pszNullClass = pszClass == NULL ? "" : pszClass;

        if (NULL != pszFile) {

            if (fTraceEn & MOFINE_STRIPPATH /*CDE_REMOVEPATHFROMFILENAME*/)
            {
                char* p = pszFile;

                while (*p != '\0')
                {
                    if (*p++ == '\\')
                    {
                        pszFile = p;
                    }
                }

            }

            _cdeCoreSprintf(pCdeAppIf, szLine, "%d", nLine);

            if (fTraceEn & MOFINE_RAWFORMAT) {
                nRet = _cdeCoreMofine(pCdeAppIf, NULL, NULL, 0, NULL, NULL, ~(MOFINE_EXITONCOND + MOFINE_DEADONCOND) & fTraceEn | 1, "%s%s%s%s%s%s%s%s%010d%s%s%s", pszDriver, &_gCdeCfgMofineRawSeparator[0],
                    pszFile, &_gCdeCfgMofineRawSeparator[0],
                    szLine, &_gCdeCfgMofineRawSeparator[0],
                    pszFunction, &_gCdeCfgMofineRawSeparator[0],
                    _cdeCoreClock(pCdeAppIf), &_gCdeCfgMofineRawSeparator[0],
                    pszNullClass, &_gCdeCfgMofineRawSeparator[0],
                    &_gCdeCfgMofineRawSeparator[0]
                    );
            }
            else {
                if (fTraceEn & MOFINE_NCLOCK) {
                    nRet = _cdeCoreMofine(pCdeAppIf, NULL, NULL, 0, NULL, NULL, ~(MOFINE_EXITONCOND + MOFINE_DEADONCOND) & fTraceEn | 1, "%s%s%s%s%s%s%s%s%s", fTraceEn & MOFINE_NDRIVER ? "" : pszDriver,
                        fTraceEn & MOFINE_NDRIVER ? "" : " ",
                        fTraceEn & MOFINE_NFILE ? "" : pszFile,
                        fTraceEn & MOFINE_NLINE ? "" : "(",
                        fTraceEn & MOFINE_NLINE ? "" : szLine,
                        fTraceEn & MOFINE_NLINE ? "" : ")",
                        fTraceEn & MOFINE_NFUNCTION ? "" : pszFunction,
                        fTraceEn & MOFINE_NFUNCTION ? "" : "()",
                        fTraceEn & MOFINE_NCLASS ? "" : pszNullClass
                        );
                }
                else {
                    nRet = _cdeCoreMofine(pCdeAppIf, NULL, NULL, 0, NULL, NULL, ~(MOFINE_EXITONCOND + MOFINE_DEADONCOND) & fTraceEn | 1, "%s%s%s%s%s%s%s%s%010d%s%s", fTraceEn & MOFINE_NDRIVER ? "" : pszDriver,
                        fTraceEn & MOFINE_NDRIVER ? "" : " ",
                        fTraceEn & MOFINE_NFILE ? "" : pszFile,
                        fTraceEn & MOFINE_NLINE ? "" : "(",
                        fTraceEn & MOFINE_NLINE ? "" : szLine,
                        fTraceEn & MOFINE_NLINE ? "" : ")",
                        fTraceEn & MOFINE_NFUNCTION ? "" : pszFunction,
                        fTraceEn & MOFINE_NFUNCTION ? "" : "()",
                        _cdeCoreClock(pCdeAppIf),
                        fTraceEn & MOFINE_NCLASS ? "" : " ",
                        fTraceEn & MOFINE_NCLASS ? "" : pszNullClass
                        );
                }
            }
        }
        
        //
        // support MOFINE_STDOUT, MOFINE_STDERR, MOFINE_STATUSCODE
        //
        if (1)
        {
            int i;
            _PUTCHAR*   tracefunc[] = { 
                pCdeAppIf->pCdeServices->pPutDebug, 
                pCdeAppIf->pCdeServices->pPutConOut, 
                pCdeAppIf->pCdeServices->pPutConOut 
            };
            static int  traceflag[] = { 
                MOFINE_STATUSCODE , 
                MOFINE_STDOUT, 
                MOFINE_STDERR 
            };

            for (i = 0; i < sizeof(tracefunc) / sizeof(tracefunc[0]); i++)
            {
                if (fTraceEn & traceflag[i])
                {
                    nRet += (int)pCdeAppIf->pCdeServices->pVwxPrintf(
                        pCdeAppIf,          //this
                        &RomParm,           //IN ROMPARM_VWXPRINTF *pRomParm,
                        pszFormat,          //IN const void *pszFormat,
                        //tracefunc[i],       //void (*pfnDevPutChar)(UINT16/*wchar_t*/c,void** ppDest/*address of pDest*/),
                        pCdeAppIf->pCdeServices->pPutConOut,
                        pCdeAppIf,          //CDE_APP_IF *CdeAppIf parm for pPutDebug == _CdeDbgPutChar()
                        (unsigned)-1,       //unsigned dwCount,
                        ap                  //IN va_list ap
                    );
                }
            }
        }
    }//if(fTraceEn == 1;

    if (fTraceEn & MOFINE_EXITONCOND)
        _cdeCoreExit(pCdeAppIf, 128);

    if (fTraceEn & MOFINE_DEADONCOND) {
        volatile int x = 0;
        while (0 == x)
            ;
    }
    return nRet;

}
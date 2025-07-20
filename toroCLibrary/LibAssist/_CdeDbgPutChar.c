/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _CdeDbgPutChar.c

Abstract:

    CDE internal send character to TRACE/ReportStatusCode output.

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>
#include <stdio.h>

extern unsigned char _gCdeCfgCmdLnParmReportStatusCodeSTDOUT;
extern char __cdeGetCurrentPrivilegeLevel(void);
//extern void EFIAPI _CdeEDKDebugPrint (IN const char *pChar);
extern int _CdeDbgCOMInit(void);
extern int _CdeDbgCOMReady2Send(void);
extern int _CdeDbgCOMSend(short);

typedef struct _DEBUGNFO {
    EFI_STATUS_CODE_DATA Header;
    unsigned  ErrorLevel;
    unsigned long long            parm[12];
    char             szText[4];
}DEBUGNFO;

void _CdeDbgPutChar(short c, void** ppDest) {

    DEBUGNFO DbgNfo = {
                    .Header.HeaderSize = sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Size = sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
                    .Header.Type = { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
                    .ErrorLevel = 0xFF,
                    .parm = {0,0,0,0,0,0,0,0,0,0,0,0},
                    .szText = "ABC"
    };
    do {
        CDE_APP_IF* pCdeAppIf = (CDE_APP_IF*)*ppDest;

        if (EOF == c)               // skip writing EOF that is send by printf() for flushing to stdout
            break;

        if (_gCdeCfgCmdLnParmReportStatusCodeSTDOUT == 1 && CDE_STATUSCODE2STDOUTBYCMDLN)
        {

            pCdeAppIf->pCdeServices->pPutConOut(c, (void**)&pCdeAppIf);

            if (c == '\n' && CDE_CARRIAGERETURN_AFTER_LINEFEED)
            {
                int r = '\r';
                pCdeAppIf->pCdeServices->pPutConOut(r, (void**)&pCdeAppIf);
            }
        }

        if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
            
            if (FALSE == pCdeAppIf->pCdeServices->fCOM1Timeout)
            {
                int i = 0;
                DbgNfo.szText[0] = '\n' == (char)c ? '\r' : (char)c;
                DbgNfo.szText[1] = '\n' == (char)c ? '\n' : '\0';
                DbgNfo.szText[2] = '\0';
                unsigned long long timeout = pCdeAppIf->pCdeServices->TSClocksPerSec / 100 + __rdtsc();     // timeout 10ms  (TSClocksPerSec/100)

                if (_CdeDbgCOMInit()) {
                    char fTimeout = FALSE;
                    do {

                        while (0 == _CdeDbgCOMReady2Send())
                        {
                            if (timeout < __rdtsc())
                            {
                                pCdeAppIf->pCdeServices->fCOM1Timeout = TRUE;
                                fTimeout = TRUE;
                                break;
                            }
                        }

                        if (TRUE == fTimeout)
                            break;

                        _CdeDbgCOMSend(DbgNfo.szText[i]);

                    } while (DbgNfo.szText[++i] != '\0');
                }
            }

        }
        else {

            DbgNfo.szText[0] = (char)c;
            DbgNfo.szText[1] = '%' == (char)c ? '%' : '\0'; // allow single '%' to be printed by using ReportStatus engine
            DbgNfo.szText[2] = '\0';

            if (NULL != pCdeAppIf->pCdeServices->ReportStatusCode.pPei) {
                switch (pCdeAppIf->DriverParm.CommParm.OSIf) {
                case PEIIF:/*    PEIIF,DXEIF,SMMIF,SHELLIF*/

                    pCdeAppIf->pCdeServices->ReportStatusCode.pPei(
                        (void*)pCdeAppIf->DriverParm.PeimParm.PeiServices, /*IN const EFI_PEI_SERVICES **PeiServices,*/
                        EFI_DEBUG_CODE,
                        (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED),
                        0,
                        NULL,
                        (void*)&DbgNfo);
                    break;
                default:
                    (*pCdeAppIf->pCdeServices->ReportStatusCode.pDxe)(
                        EFI_DEBUG_CODE,
                        (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED),
                        0,
                        NULL,
                        (void*)&DbgNfo);
                    break;
                }
            }
        }
    }while(0);
}

#pragma warning (disable:4206)
///*++
//
//    toro C Library
//    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library
//
//    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
//    SPDX-License-Identifier: GNU General Public License v3.0
//
//Module Name:
//
//    _CdeEDKDebugPrint.c
//
//Abstract:
//
//        CDE internal send character to ReportStatusCode output.
//
//Author:
//
//    Kilian Kegel
//
//--*/
//#include <Uefi.h>
//#include "Pi\PiStatusCode.h"
//#include <Protocol\StatusCode.h>
//#include <stdio.h>
//
//extern EFI_STATUS_CODE_PROTOCOL *pgEfiStatusCodeProtocol;
//
//typedef struct _DEBUGNFO{
//      EFI_STATUS_CODE_DATA Header;
//      unsigned  ErrorLevel;
//      UINT64            parm[12];
//      char             szText[4];
//}DEBUGNFO;
//
////typedef struct _ASSERTNFO{
////      EFI_STATUS_CODE_DATA Header;
////      EFI_DEBUG_ASSERT_DATA Adata; //only LineMumber used here
////      char   szFilename[256];
////  }ASSERTNFO;
////
//#define QWORDPARM01 0x1111111111111111
//#define QWORDPARM02 0x2222222222222222
//#define QWORDPARM03 0x3333333333333333
//#define QWORDPARM04 0x4444444444444444
//#define QWORDPARM05 0x5555555555555555
//#define QWORDPARM06 0x6666666666666666
//#define QWORDPARM07 0x7777777777777777
//#define QWORDPARM08 0x8888888888888888
//#define QWORDPARM09 0x9999999999999999
//#define QWORDPARM00 0xAAAAAAAAAAAAAAAA
//#define QWORDPARM11 0xBBBBBBBBBBBBBBBB
//#define QWORDPARM12 0xCCCCCCCCCCCCCCCC
//
//void EFIAPI _CdeEDKDebugPrint (IN const char  *pChar){
//static DEBUGNFO DbgNfo={
//                          sizeof(EFI_STATUS_CODE_DATA),
//                          sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
//                          { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
//                          0xFF,
//                          {
//                            QWORDPARM01,QWORDPARM02,QWORDPARM03,QWORDPARM04,QWORDPARM05,QWORDPARM06,
//                            QWORDPARM07,QWORDPARM08,QWORDPARM09,QWORDPARM00,QWORDPARM11,QWORDPARM12
//                          },
//                          "ABC"
//                        };
//
//
////      printf("%c",(UINT8)*pChar);
//        DbgNfo.szText[0]=*pChar;
//        DbgNfo.szText[1]='\0';
//
//
//      (pgEfiStatusCodeProtocol->ReportStatusCode)(
//      EFI_DEBUG_CODE,
//      (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED),
//      0,
//      NULL,
//      (void*)&DbgNfo);
//}
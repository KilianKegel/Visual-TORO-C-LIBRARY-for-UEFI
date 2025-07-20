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
//    _CdeXDump.c
//
//Abstract:
//
//    CDE internal configurable hex dump routine
//
//Author:
//
//    Kilian Kegel
//
//--*/
//#include <CdeServices.h>
//
///** _CdeXDump()
//
//Synopsis
//
//    int _CdeXDump(  XDUMPPARM ctrl, 
//                    unsigned elmcount, 
//                    unsigned long long startaddr, 
//                    unsigned long long(*pfnGetElm)(unsigned long long qwAddr), 
//                    unsigned (*pfnWriteStr)(char* szLine),
//                    char* pBuf, 
//                    int bufsize);
//
//Description
//
//    change current directory
//
//Returns
//
//    @param[in] ctrl         : control word
//    @param[in] elmcount     : element count
//    @param[in] startaddr    : start address
//    @param[in] pfnGetElm    : get element routine
//    @param[in] pfnWriteStr  : call back routine to print a string
//    
//    @retval int number of bytes written
//
//**/
//int _CdeXDump(XDUMPPARM ctrl, unsigned elmcount, unsigned long long startaddr, unsigned long long(*pfnGetElm)(unsigned long long qwAddr), unsigned (*pfnWriteStr)(char* szLine),char* pBuf, int bufsize) {
//    int nRet = 0;
//    CDE_APP_IF* pCdeAppIf = __cdeGetAppIf();
//
//    if (pCdeAppIf) do
//    {
//        nRet = pCdeAppIf->pCdeServices->pXDump(pCdeAppIf, ctrl, elmcount, startaddr, pfnGetElm, pfnWriteStr, pBuf, bufsize);
//    } while (0);
//
//    return nRet;
//}
#include <CdeServices.h>
static char filler[] = { " warning C4206: nonstandard extension used: translation unit is empty" };

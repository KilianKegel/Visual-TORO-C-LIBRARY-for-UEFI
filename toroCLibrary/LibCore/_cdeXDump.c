/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeXDump.c

Abstract:

    CDE internal diagnostic support

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <CdeServices.h>
#include <CDE.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

extern int _cdeCoreSprintf(CDE_APP_IF* pCdeAppIf, char* pszDest, const char* pszFormat, ...);
extern int _cdeCoreMofine(CDE_APP_IF* pCdeAppIf, char* pszDriver, char* pszFile, int nLine, char* pszFunction, char* pszClass, int fTraceEn, char* pszFormat, ...);

DIAGXDUMP        _cdeXDump;//prototype

static const char szTwoSpaces[] = { 0x20,'\0' };
static const char szOneSpace[] = { 0x20,'\0' };

/*!
    @fn static int PrintAscii(char *pBuffer, unsigned elmsize, unsigned cToPrint,unsigned char *pTextLineBuf)

    @brief prints the ASCII interpretation binary byte/word/dword/qword

    @details Features:

    @param[in] *pBuffer : pointer to binary buffer
    @param[in] elmsize : element size
    @param[in] cToPrint : count to print
    @param[in] *pTextLineBuf : pointer to text buffer

    @return 0
*/
static int PrintAscii(CDE_APP_IF* pCdeAppIf, char* pBuffer, unsigned elmsize, unsigned cToPrint, unsigned char* pTextLineBuf) {
    unsigned char* pb = (unsigned char*)&pBuffer[0];
    unsigned short* pw = (unsigned short*)&pBuffer[0];
    unsigned int* pdw = (unsigned int*)&pBuffer[0];
    unsigned long long* pqw = (unsigned long long*) & pBuffer[0];
    unsigned long long qwLit2Big;/*!< little endian to big endian translation buffer*/
    unsigned char* pLit2Big = (unsigned char*)&qwLit2Big;
    unsigned j;

#define PRINTREVERSE for (k = elmsize - 1 ; k != (unsigned)-1 ; k--){\
                             _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)],"%c", isalnum(pLit2Big[k]) ? 0xFF & pLit2Big[k] : '.'); \
                         }\
                         if (elmsize - 1){/*!< add space between ASCII char, except in 8-bit format*/\
                             _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)]," ");\
                         }// END PRINTREVERSE

    switch (elmsize) {
        unsigned k;
        case sizeof(char) :
            for (j = 0; j < cToPrint; j++) {
                *((unsigned char*)pLit2Big) = 0xFF & pb[j];
                PRINTREVERSE;
            }
        break;

        case sizeof(short) :
            for (j = 0; j < cToPrint; j++) {
                *((unsigned short*)pLit2Big) = 0xFFFF & pw[j];
                PRINTREVERSE;
            }
        break;

        case sizeof(int) :
            for (j = 0; j < cToPrint; j++) {
                *((unsigned int*)pLit2Big) = 0xFFFFFFFF & pdw[j];
                PRINTREVERSE;
            }
        break;

        case sizeof(long long) :
            for (j = 0; j < cToPrint; j++) {
                *((unsigned long long*)pLit2Big) = 0xFFFFFFFFFFFFFFFFLL & pqw[j];
                PRINTREVERSE;
            }
        break;
    }
    return 0;
}

/*!
    @fn int _cdeXDump(CDE_APP_IF* pCdeAppIf, XDUMPPARM ctrl, unsigned elmcount, unsigned long long startaddr, unsigned long long(*pfnGetElm)(unsigned long long qwAddr), unsigned (*pfnWriteStr)(char* szLine), char* pBuf, unsigned bufsize )
    
    @brief dump an addressrange, highly configurable

    @details Features:
        1. w/ or w/o appended ASCII translation -> XDUMPPARM.bit.NoAscii
        2. byte/word/dword and qword support    -> XDUMPPARM.bit.elmsize
        3. configurable/enabe/disable address size printed at begin of each line -> XDUMPPARM.bit.AddrSize
        4. configurable bytes per lane + 1, until 128 -> XDUMPPARM.bit.BytesPerLineMin1
        5. configurable base and offset only print    -> XDUMPPARM.bit.BaseOfs
        6. configurable dash character "-" at half of the line -> XDUMPPARM.bit.NoDash
        7. configurable Pitch to next character -> XDUMPPARM.bit.Pitch

    @param[in] ctrl : control word
    @param[in] elmcount : count bytes/words/dwords/qwords per line
    @param[in] startaddr : start address
    @param[in] pfnGetElm : get element routine
    @param[in] pfnWriteStr : call back routine to print a string
    @param[in] pbuf : pointer to buffer for internal text line processing
    @param[in] bufsize : size of the buffer

    @return 0 on success
    @return positive number on buffer size fail with required buffer size
    @return INT_MIN
*/
int _cdeXDump(
    CDE_APP_IF* pCdeAppIf, 
    XDUMPPARM ctrl, 
    unsigned elmcount, 
    unsigned long long startaddr, 
    unsigned long long(*pfnGetElm)(unsigned long long qwAddr), 
    unsigned (*pfnWriteStr)(char* szLine), char* pBuf,
    unsigned bufsize /*buffer size*/
    )
{
    unsigned size = 0;
    unsigned nRet = 0;
    unsigned elmsize = 1 + ctrl.bit.ElmSizeMin1;
    unsigned Pitch = ctrl.bit.Pitch;
    unsigned u,i/*<unsigned index*/;
    unsigned ElmsPerLine = (0 == ctrl.bit.ElmsPerLine ? 16 : ctrl.bit.ElmsPerLine);
    unsigned ElmsPerLine2 = ElmsPerLine/2;
    unsigned char *pBuffer = pBuf;
    unsigned char *pTextLineBuf;
    unsigned char      *pb  = (unsigned char*)     &pBuffer[0];
    unsigned short     *pw  = (unsigned short*)    &pBuffer[0];
    unsigned int       *pdw = (unsigned int*)      &pBuffer[0];
    unsigned long long *pqw = (unsigned long long*)&pBuffer[0];
    static char *rgszAddrXX[] = { { "%016llX: " },{ "%08llX: " },{ "%04llX: " },{ "%02llX: " },{ "" } };/*<address field size strings*/
    char *pAddrXX = rgszAddrXX[ctrl.bit.AddrSize];
    
    do {
        switch (ctrl.bit.AddrSize) {
            case 0: size = sizeof("0123456789ABCDEF: ") - 1; break;
            case 1: size = sizeof("01234567: ") - 1;        break;
            case 2: size = sizeof("0123: ") - 1;            break;
            case 3: size = sizeof("01: ") - 1;              break;
            case 4: size = sizeof("") - 1;                  break;
            default:size = sizeof("0123456789ABCDEF: ") - 1;
        }
        size += (elmsize * 2 + 1) * ElmsPerLine;                // each byte of an element needs 2 bytes to be represented in HEXASCII, one (1) additional space
        size += sizeof((char)'\n');                             // line is completed by a new line character
        size += sizeof((char)'\0');                             // line is terminated by a 0
        size += (0 == ctrl.bit.NoDash ? sizeof("- ") - 1 : 0);
        if (0 == ctrl.bit.NoAscii) {
            size += 2;                                          // ...additionally in ASCII mode two(2) spaces are in use
            size += elmsize * ElmsPerLine;                      // each byte of an element needs 1 bytes to be represented in pure ASCII
            size += ElmsPerLine * (elmsize > 1 ? 1 : 0);        // each element is separated by a single space, accept the last one...
        }

        size += ElmsPerLine * elmsize;                          // additional binary buffer

        if ((NULL == pBuf) || (size > bufsize)) {
            nRet = size;
            break;
        }
        COREMOFINE((COREINF(1) "size -> %d\n", size));

        pTextLineBuf = &pBuffer[ElmsPerLine * elmsize];

        pTextLineBuf[0] = '\0';

        for (u = 0, i = 0; u < elmcount; u += (elmsize + Pitch), i++) {

            if (0 == ctrl.bit.NoDash)
                if (ElmsPerLine2)
                    if (0 == ((i) % ElmsPerLine2) && 0 != ((i) % ElmsPerLine))
                        _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "- ");

            if (0 == (i % ElmsPerLine))
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], pAddrXX, u + (1 == ctrl.bit.BaseOfs ? startaddr : 0LL));

            switch (elmsize) {
            case 1: pb[(i % ElmsPerLine)] = (unsigned char)(*pfnGetElm)(startaddr + u);
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "%02X ", 0xFF & pb[(i % ElmsPerLine)]);
                break;
            case 2: pw[(i % ElmsPerLine)] = (unsigned short)(*pfnGetElm)(startaddr + u);
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "%04X ", 0xFFFF & pw[(i % ElmsPerLine)]);
                break;
            case 4: pdw[(i % ElmsPerLine)] = (unsigned int)(*pfnGetElm)(startaddr + u);
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "%08X ", 0xFFFFFFFF & pdw[(i % ElmsPerLine)]);
                break;
            case 8: pqw[(i % ElmsPerLine)] = (unsigned long long)(*pfnGetElm)(startaddr + u);
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "%016llX ", 0xFFFFFFFFFFFFFFFFLL & pqw[(i % ElmsPerLine)]);
                break;
            }

            if (0 == ((i + 1) % ElmsPerLine)) {
                //
                // print ascii values
                //
                if (0 == ctrl.bit.NoAscii) {
                    _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], szTwoSpaces);
                    PrintAscii(pCdeAppIf, &pBuffer[0], elmsize, ElmsPerLine, &pTextLineBuf[strlen(pTextLineBuf)]);
                }
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "\n");
                pfnWriteStr(pTextLineBuf);
                pTextLineBuf[0] = '\0';
            }
        }
        //
        // print remaining ASCII chars
        //
        if (1) {
            unsigned rem = i % ElmsPerLine                                              /*remining hex numbers (filled w/ space)*/;
            unsigned asc = ElmsPerLine - rem                                            /*ASCII characters not yet printed*/;
            unsigned hex = asc / 1/*elmsize*/                                           /*HEX numbers not yet printed == asc / elmsize*/;
            unsigned cSpaces = hex * elmsize * 2 + hex                                  /*count spaces between the single numbers, depending on printsize*/;
            unsigned cDashSpace = ctrl.bit.NoDash ? 0 : (rem > ElmsPerLine2 ? 0 : 2);  /*count dash and space 0 / 2, depending on HEXDUMPPARMS.bit.NoDash*/


            if (0 != rem && 1 == ctrl.bit.NoAscii)
                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "\n");

            if (0 == ctrl.bit.NoAscii) {
                if (rem) {
                    unsigned x;
                    for (x = 0; x < cSpaces + cDashSpace; x++) {
                        _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], szOneSpace);
                    }
                    _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], szTwoSpaces);
                }

                PrintAscii(pCdeAppIf, &pBuffer[0], elmsize, rem, &pTextLineBuf[strlen(pTextLineBuf)]);

                _cdeCoreSprintf(pCdeAppIf, &pTextLineBuf[strlen(pTextLineBuf)], "\n");
                pfnWriteStr(pTextLineBuf);
                pTextLineBuf[0] = '\0';
            }
        }
    } while (0);

    return (int)nRet;
}

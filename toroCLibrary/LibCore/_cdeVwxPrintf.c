/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeVwxPrintf.c

Abstract:

    CDE internal worker function for the printf()-family

Author:

    Kilian Kegel

--*/
#include <Cde.h>
#include <CdeServices.h>
#include <stdarg.h>
#include <stdint.h>

//
// prototype
///
VWXPRINTF _cdeVwxPrintf;
extern void _CdeMemPutChar(int c, void** ppDest);
extern int _cdeCoreSprintf(CDE_APP_IF* pCdeAppIf, char* pszDest, const char* pszFormat, ...);

#define BUFFLEN (64 + 2)
#define XWORD unsigned long long
#define SXWORD long long

#define STRLEN(pszStr) (fWide ? _wcslen((short*)pszStr) : _strlen(pszStr))

typedef struct _NUMDESC {
    IN int nNumBase;
    IN short nZeroExtendedLen;
    IN unsigned char fSigned;
    IN unsigned char fLowerCase;
    IN SXWORD nNumber;
    IN char* pszNumber;
    OUT char* pszSS; //Sign/Sharp
} NUMDESC;

typedef struct _FLAGS {
    unsigned int fSharp, fLZero, fLBlank, fMinus, fPlus, fPrecision, fWideStringOvr;
    int nFieldsize, nPrecisionsize;//20111227 fixed wrong fieldsize if nPrecisionsize is unsigned
} FLAGS;


char gszEmpty[] = { "" };

unsigned int _strlen(const char* pszBuffer) {
    int i = 0;

    while (pszBuffer[i] != '\0')
        i++;
    return i;
}

unsigned int _wcslen(const short* pszBuffer) {
    unsigned int i = 0;

    while (pszBuffer[i] != '\0')
        i++;
    return i;
}

static int str2dev(void (*pfnDevPutChar)(int, void**), char* pszBuf, unsigned int* pCount, void** ppDest, unsigned int nMaxnum, unsigned char fWide, unsigned char IsSingle/*Char->allow '\0' to be printed*/) {
    unsigned int nCnt, nEnd;
    int nRet = 0;
    short c;

    nEnd = IsSingle ? 1 : (int)STRLEN(pszBuf);
    nEnd = nEnd > nMaxnum ? nMaxnum : nEnd;

    for (nCnt = 0; nCnt < nEnd && *pCount; nCnt++) {
        (*pCount)--;
        nRet++;
        c = (fWide ? ((short*)pszBuf)[nCnt] : pszBuf[nCnt]);
        if (pfnDevPutChar != NULL)
            (*pfnDevPutChar)(c, ppDest);
    }
    return nRet;
}


int ComputeFieldSize(char* pfieldsizebuf) {
    int i = -1;
    int nRet = 0;
    int factor = 1;

    do {
        while ((char)-1 != pfieldsizebuf[++i])
            ;
        while (i--) {                                 // while i != 0 decrement i
            nRet += factor * pfieldsizebuf[i];
            factor *= 10;
        };
    } while (0);
    return nRet;
}

static char* num2str(NUMDESC* pParms, unsigned char fWide) {
    int i = BUFFLEN - 1;
    unsigned char fNegative = FALSE;
    XWORD nNumber, nNumberoriginal;
    short nZeroExtendedLen = pParms->nZeroExtendedLen;
    char c;

    fWide ? (((short*)(pParms->pszNumber))[i--] = '\0') : (pParms->pszNumber[i--] = '\0');

    //            if(   (pParms->nNumBase != 16) && (pParms->nNumBase != 10) && (pParms->nNumBase != 8) && (pParms->nNumBase != 2))
    //                break;

    if (pParms->fSigned == TRUE && pParms->nNumber < 0) {
        fNegative = TRUE;
        nNumber = (XWORD)-pParms->nNumber;
    }
    else {
        nNumber = (XWORD)pParms->nNumber;
    }

    nNumberoriginal = nNumber;

    do {
        c = (char)(((XWORD)((nNumber) % (XWORD)pParms->nNumBase)) + '0');
        if (c > '9')                                                         // correct if hex
            c += (('A' - '9' - 1) | pParms->fLowerCase << 5);
        fWide ? (((short*)(pParms->pszNumber))[i] = c) : (pParms->pszNumber[i] = c);
        nNumber = (nNumber) / pParms->nNumBase;
        nZeroExtendedLen--;
    } while (--i && (nNumber || ((nZeroExtendedLen) > 0)));

    pParms->pszSS[0] = '\0';
    pParms->pszSS[1] = '\0';
    pParms->pszSS[2] = '\0';

    if (pParms->fSigned == TRUE) {
        pParms->pszSS[0] = fNegative == TRUE ? '-' : '+';
    }
    else {
        if (nNumberoriginal) {
            if (pParms->nNumBase == 16) {
                pParms->pszSS[0] = '0';
                pParms->pszSS[1] = 'X';
                pParms->pszSS[1] |= pParms->fLowerCase << 5;
            }
            if (pParms->nNumBase == 8)
                pParms->pszSS[0] = '0';
        }
    }

    return fWide ? (char*)&((short*)(pParms->pszNumber))[i + 1] : &pParms->pszNumber[i + 1];
}


static int nfill(char c, int n, unsigned int* pCount, void** ppDest, void (*pfnDevPutChar)(int, void**)) {
    int nRet = 0, i;

    for (i = 0; i < n && *pCount; i++) {
        (*pCount)--;
        nRet++;
        if (pfnDevPutChar != NULL)
            (*pfnDevPutChar)(c, ppDest);
    }
    return nRet;
}

static int nprintfield(
    char* pszStr,
    char* pszSSIn,
    FLAGS* pFlags,
    void (*pfnDevPutChar)(int, void**),
    unsigned int* pCount,
    void** ppDest,
    unsigned char fWide,
    unsigned char IsSingle/*KG20170117 Char->allow '\0' to be printed*/)
{

    int nRet = 0;
    char filler = pFlags->fLZero ? '0' : '\x20';
    char* pszSS = &gszEmpty[0];
    int nStrlen, nSSlen, nFillerlen, nPrecilen;
    unsigned char fTrailingBlanks, fLeadingBlanks, fFillingZeros;
    unsigned char fStringIsString = (pszSSIn == NULL);   // pszStr: is it a string or a number???

    pszSS = pszSSIn == NULL ? pszSS : pszSSIn;                    // if number, set sign/sharp string

    if (pFlags->fLBlank && pFlags->fPlus)   // fix "%+ d" / "% +d"
        pFlags->fLBlank = 0;                // fix "%+ d" / "% +d"
    if (pFlags->fLZero && pFlags->fMinus)   // fix "%0-2i"
        pFlags->fLZero = 0;                 // fix "%0-2i"

    if (pszSS[0] != '+' && pszSS[0] != '-') {                           // check for signed integer
        pszSS[0] = (!pFlags->fSharp) ? '\0' : pszSS[0];                 // replace S/# by / if no #-flag is set
                                                                        // (dont print 0x on hex if # is absent)!!!
    }

    pFlags->fLZero = (STRLEN(pszStr) >= (unsigned)pFlags->nFieldsize) ? 0 : pFlags->fLZero; //20111227 W/A printf("|%05d|\n",100000);

    if (pszSS[0] == '+') {                                                // if signed, positive number, replace '+'  by space, '0' or cut to zero string
//        pszSS[0] = (pFlags->fLBlank && !pFlags->fPlus) ? '\0' : pszSS[0];
        pszSS[0] = (pFlags->fLBlank && !pFlags->fLZero) ? 0x20 : pszSS[0];
        pszSS[0] = (pFlags->fLBlank &&  pFlags->fLZero) ? 0x20 : pszSS[0];    // fix "% 02i"
        pszSS[0] = (!pFlags->fLBlank && pFlags->fLZero && !pFlags->fPlus) ? '0' : pszSS[0];
        pszSS[0] = (!pFlags->fLBlank && !pFlags->fLZero && !pFlags->fPlus) ? '\0' : pszSS[0];
    }

    fTrailingBlanks = pFlags->fMinus ? TRUE : FALSE;
    fLeadingBlanks = !pFlags->fMinus && !pFlags->fLZero ? TRUE : FALSE;    //20111227 W/A printf("|%032.3d|\n",12); vs. printf("|%-5.0d|\n",111);
    fFillingZeros = !pFlags->fMinus && pFlags->fLZero && !pFlags->fPrecision ? TRUE : FALSE;

    // ----- W/A for precision on strings

    if (fStringIsString) {                    // have string??? (no sign/sharp string)???
        unsigned tmplen = STRLEN(pszStr);     // KG20170117 fixed: %c if char == '\0' doesn't print the '\0'

        if (IsSingle && '\0' == pszStr[0])   // KG20170117 fixed: %c if char == '\0' doesn't print the '\0'
            tmplen++;                       // KG20170117 fixed: %c if char == '\0' doesn't print the '\0'
        nStrlen = pFlags->fPrecision && (unsigned)pFlags->nPrecisionsize < tmplen ? pFlags->nPrecisionsize : tmplen;
        nPrecilen = 0;
    }
    else {
        nStrlen = STRLEN(pszStr);
        nPrecilen = pFlags->fPrecision && pFlags->nPrecisionsize - nStrlen > 0 ? pFlags->nPrecisionsize - nStrlen : 0;
    }

    // ----- W/A for Zero precision on ZERO printf("|%5.0d|\n",0);

    if (0 == fStringIsString && pFlags->fPrecision && (pFlags->nPrecisionsize == 0)) {                // have a number???
        if (('0' == (fWide ? ((short*)pszStr)[0] : pszStr[0])) && ('\0' == (fWide ? ((short*)pszStr)[1] : pszStr[1]))) {
            if (fWide)
                ((short*)pszStr)[0] = 0x20;
            else
                pszStr[0] = 0x20;
        }
    }

    nSSlen = _strlen(pszSS);
    nFillerlen = pFlags->nFieldsize - nStrlen - nSSlen - nPrecilen < 0 ? 0 : pFlags->nFieldsize - nStrlen - nSSlen - nPrecilen;

    //
    // (L) S/# (Z/P) STR (T)
    // (Leading) S/# == 0,0x,0X,+,-,zerosize String (Zero) (Precision) STR (Trailing)
    //

        // ----- W/A for leading 0 on octals

    if (pszSS[0] == '0' && pszSS[1] == '\0' && nPrecilen != 0) {
        pszSS[0] = '\0';            // cut string (leading '0')
        nFillerlen++;
    }

    if (fLeadingBlanks)                                  // (L)
        nRet += nfill(0x20, nFillerlen, pCount, ppDest, pfnDevPutChar);

    nRet += str2dev(pfnDevPutChar, pszSS, pCount, ppDest, (unsigned int)-1, 0, 0);      // S/#

    if (fFillingZeros)                                   // (Z)
        nRet += nfill(filler, nFillerlen, pCount, ppDest, pfnDevPutChar);

    if (pFlags->fPrecision && fStringIsString == 0)      // P
        nRet += nfill('0', nPrecilen, pCount, ppDest, pfnDevPutChar);

    nRet += str2dev(pfnDevPutChar, pszStr, pCount, ppDest, nStrlen, fWide, IsSingle);    // Str

    if (fTrailingBlanks)                                 // (T)
        nRet += nfill(0x20, nFillerlen, pCount, ppDest, pfnDevPutChar);

    return nRet;
}

short ydes(unsigned char fDes, int i, short v) {
    return v;
}

int
_cdeVwxPrintf(
    CDE_APP_IF* pCdeAppIf,
    IN ROMPARM_VWXPRINTF* pFixParm,
    IN const void* pszFormat,
    void (*pfnDevPutChar)(int c, void** ppDest/*address of pDest*/),
    void* pDest,
    unsigned int dwCountParm,
    IN va_list ap
) {
    unsigned char fWide = pFixParm->fWide;
    unsigned char fDes = 2 == (pFixParm->fForceToDataSeg & 2);
    int i, t;
    NUMDESC numdesc = { (unsigned char)-1,(short)-1,(unsigned char)-1,(unsigned char)-1,(SXWORD)-1,NULL,NULL };
    FLAGS flags;
    unsigned char bIntOrLongOrInt64 = (unsigned char)-1;
    char rgbBuffer[sizeof(short) * BUFFLEN];
    char rgSS[3];               // -,+,0,0x,0X         Sign/Sharp#
    char fieldsizebuf[8];
    int  ifieldsizebuf = -1;
    int* pSize = &flags.nFieldsize;    //pointer to eighter  flags.nFieldsize or flags.nPrecisionsize

    unsigned int dwInitialCount = pFixParm->fCountIsParm ? dwCountParm : -1;
    unsigned int dwCount = dwInitialCount;

    short* pszFormatW, c;
    enum STATE {
        PROCESS_FLAGS,
        PROCESS_OPSIZE,
        PROCESS_BASE,
        PROCESS_PRECISION,
        PROCESS_FIELDSIZE,
        PROCESS_THE_NUMBER_ITSELF,
        PROCESS_THE_PRECISITION_ITSELF,
        PROCESS_CHARSSTRING,
        PROCESS_PERCENT,
        PROCESS_WRITECHARS,
        PROCESS_COUNT,
        PROCESS_ERROR,
        PROCESS_THE_POINTER_ITSELF,
        PROCESS_GUID,
        PROCESS_TIME,
        PROCESS_STATUS,
        PROCESS_DONT_WRITE_ANYMORE/*kg20150210_00*/
    }state = PROCESS_WRITECHARS;

    //int snprintf( char *buffer,size_t count,  const char *format [,argument] ... );
    //int sprintf(  char *buffer,               const char *format [,argument] ... );
    //int fprintf(  FILE *stream,               const char *format [,argument] ... );
    //int cprintf(                              const char *format [,argument] ... );
    //int printf(                               const char *format [,argument] ... );


    numdesc.pszNumber = rgbBuffer;
    numdesc.pszSS = rgSS;

    pszFormatW = (short*)pszFormat;
#define CHAR816(IX) fWide ? pszFormatW[IX] : ((char*)pszFormat)[IX]
    for (i = 0, c = ydes(fDes, i, CHAR816(i)); c != '\0' && (dwCount <= dwInitialCount)/*kg20150210_00*/; c = ydes(fDes, i, CHAR816(i))) {

        switch (state) {
        case PROCESS_WRITECHARS:
            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case '%':   numdesc.nNumBase = 10;
                numdesc.fSigned = FALSE;
                numdesc.nZeroExtendedLen = -1;
                //TODO: bIntOrLongOrInt64 should be init to ZERO here
                //      and if not set explicitly by type length (h/l)
                //      on INT/char/string processing set to default type length
                //      that is "h" for char/strings
                //              "l"/"q/I64" for integers
                bIntOrLongOrInt64 = 0;

                flags.fSharp = flags.fLZero = flags.fLBlank = flags.fMinus = flags.fPlus = flags.fPrecision = 0;
                flags.fWideStringOvr = 0;

                flags.nFieldsize = 0;                   // pre adjustment
                flags.nPrecisionsize = 1;               // pre adjustment
                pSize = &flags.nFieldsize;              // pre adjustment

                state = PROCESS_FLAGS;          // change state
                break;
            default:
                if (dwCount) {
                    dwCount--;
                    if (pfnDevPutChar != NULL)
                        (*pfnDevPutChar)(ydes(fDes, i - 1, CHAR816(i - 1)), &pDest);
                }
                state = dwCount ? state : PROCESS_DONT_WRITE_ANYMORE;   //set state before decrement dwCount    /*kg20150210_00*/
                break;
            }
            break;//case PROCESS_WRITECHARS:
        case PROCESS_FLAGS:
            // initilaize fieldsizebuf with -1
            for (ifieldsizebuf = 0; ifieldsizebuf < 8; ifieldsizebuf++) fieldsizebuf[ifieldsizebuf] = -1;

            ifieldsizebuf = 0;

            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case '#':   flags.fSharp = TRUE; break;
            case '0':   flags.fLZero = TRUE; break;
            case 0x20:  flags.fLBlank = TRUE; break;   // start pos. numbers with blank
            case '-':   flags.fMinus = TRUE; break;
            case '+':   flags.fPlus = TRUE; break;

            case '%':   i--; state = PROCESS_PERCENT; break;;

            case 'C':
            case 'S':   bIntOrLongOrInt64 = fWide ? 16 : 32;
                        i--; state = PROCESS_CHARSSTRING; break;
            case 's':   if (1 == pFixParm->fUEFIFormat)
                            bIntOrLongOrInt64 = 32;
                        else
                            if(0 == bIntOrLongOrInt64)
                                bIntOrLongOrInt64 = fWide ? 32 : 64;
            case 'c':   i--; state = PROCESS_CHARSSTRING; break;
            case 'a':   if (1 == pFixParm->fUEFIFormat)
                            i--, state = PROCESS_CHARSSTRING;
                        break;   /* in UEFI %a == ASCII STRING, in STDC float print */
            case 'd':
            case 'i':   numdesc.fSigned = TRUE;
            case 'b':
            case 'o':
            case 'u':
            case 'x':
            case 'X':   i--; state = PROCESS_THE_NUMBER_ITSELF; break;

            case 'p':   i--; state = PROCESS_THE_POINTER_ITSELF; break;

            case 'g':   i--;numdesc.fLowerCase = 1;state = PROCESS_GUID;break;  /* EFI_SPECIFIC*/
            case 'G':   i--;numdesc.fLowerCase = 0;state = PROCESS_GUID;break;  /* EFI_SPECIFIC*/
            case 't':   
                        //
                        // NOTE:    't' is ANSI C 99
                        // 
                        //          t   Specifies that a following d, i, o, u, x, or X conversion specifier applies to a
                        //              ptrdiff_t or the corresponding unsigned integer type argument; or that a
                        //              following n conversion specifier applies to a pointer to a ptrdiff_t
                        //              argument.
                        //
                        //          t   is misused by UEFI to print the time
                        //
                        if(1 == pFixParm->fUEFIFormat)
                        { 
                            i--; state = PROCESS_TIME; break;                   /* EFI_SPECIFIC //NOTE: THE FLAG 't' is misused by UEFI*/
                        }
                        else
                        { 
                            i--; state = PROCESS_OPSIZE; break;
                        }
                        
            case 'r':   i--;state = PROCESS_STATUS;break;                       /* EFI_SPECIFIC*/
            
            case '*':   *pSize = va_arg(ap, int);
                break;

            case '.':
                pSize = &flags.nPrecisionsize;
                flags.fLZero = 0;       /*20111227 W/A printf("|%032.3d|\n",12); vs. printf("|%-5.0d|\n",111);*/
                flags.fPrecision = 1;
                state = PROCESS_PRECISION;
                break;
            case '`':   pSize = &numdesc.nNumBase;
                state = PROCESS_BASE;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':   i--;
                state = PROCESS_FIELDSIZE;
                break;
            case 'j':
            //NOTE: THE FLAG 't' is misused by UEFI case 't':
            case 'z':
            case 'h':
            case 'l':
            case 'q':
            case 'I':   i--; state = PROCESS_OPSIZE; break;
            case 'n':   state = PROCESS_COUNT; break;

            default: state = PROCESS_ERROR; break;
            }
            break;//PROCESS_FLAGS
        case PROCESS_OPSIZE:
            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case 'h':   bIntOrLongOrInt64 = 16;
                // 'hh' ?
                if ('h' == ydes(fDes, i, CHAR816(i))) {
                    bIntOrLongOrInt64 = 8;
                    i++;
                }
                break;
            case 'l':   bIntOrLongOrInt64 = 32;

                // 'll' ?
                if ('l' == ydes(fDes, i, CHAR816(i))) {
                    bIntOrLongOrInt64 = 64;
                    i++;
                }
                else
                    flags.fWideStringOvr = 1;
                break;
            case 'q':   bIntOrLongOrInt64 = 64; break;
            case 'I':   /* I32, I64 == ms specific */
                if ('3' == ydes(fDes, i, CHAR816(i)) &&
                    '2' == ydes(fDes, i + 1, CHAR816(i + 1))) {
                    bIntOrLongOrInt64 = 32;
                    i += 2;
                }
                else
                    if ('6' == ydes(fDes, i, CHAR816(i)) &&
                        '4' == ydes(fDes, i + 1, CHAR816(i + 1))) {
                        bIntOrLongOrInt64 = 64;
                        i += 2;
                    }
                    else {
                        i--; state = PROCESS_ERROR;
                    }
                break;
            case 'j':   bIntOrLongOrInt64 = 8 * sizeof(long long); break;
            case 't':   bIntOrLongOrInt64 = 8 * sizeof(ptrdiff_t); break;
            case 'z':   bIntOrLongOrInt64 = 8 * sizeof(size_t); break;
            default: i--; state = PROCESS_FLAGS; break;
            }
            break;//PROCESS_OPSIZE
        case PROCESS_BASE:
        case PROCESS_PRECISION:
        case PROCESS_FIELDSIZE:
            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':   fieldsizebuf[ifieldsizebuf++] = (char)ydes(fDes, i - 1, CHAR816(i - 1)) - '0';
                if (ifieldsizebuf > 7)
                    state = PROCESS_ERROR;
                break;

            default:
                *pSize = ComputeFieldSize(&fieldsizebuf[0]);
                i--;
                //
                // W/A for "|%#5..3o|",01234567 /*buggy format with precision dot twice*/
                //
                if ((state == PROCESS_PRECISION) && (c == '.')) {
                    state = PROCESS_ERROR;
                    i++;   //incerement in case of error that was previously decremeted
                }
                else
                    state = PROCESS_FLAGS;

                break;
            }

            break;//PROCESS_FIELDSIZE/PROCESS_PRECISION

        case PROCESS_GUID: {      // %g
            COMM_GUID* pGuid;
            char* pFormat;

            pFormat = numdesc.fLowerCase == 0 ? "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" : \
                "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";

            pGuid = va_arg(ap, COMM_GUID*);
            _cdeCoreSprintf(pCdeAppIf, (char*)&rgbBuffer[0], pFormat, pGuid->Data1, pGuid->Data2, pGuid->Data3, pGuid->Data4[0], pGuid->Data4[1], pGuid->Data4[2], pGuid->Data4[3], pGuid->Data4[4], pGuid->Data4[5], pGuid->Data4[6], pGuid->Data4[7]);
            nprintfield((char*)&rgbBuffer[0], NULL, &flags, pfnDevPutChar, (uint32_t*)&dwCount, &pDest, fWide | (32 == bIntOrLongOrInt64), 0/*is not single*/);

            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            i++;
            break;// PROCESS_GUID

        }
        case PROCESS_TIME: {      // %t
            typedef struct {
                uint16_t  Year;
                uint8_t   Month;
                uint8_t   Day;
                uint8_t   Hour;
                uint8_t   Minute;
                uint8_t   Second;
                uint8_t   Pad1;
                uint32_t  Nanosecond;
                int16_t   TimeZone;
                uint8_t   Daylight;
                uint8_t   Pad2;
            } EFI_TIME;

            EFI_TIME* pTime;

            pTime = va_arg(ap, EFI_TIME*);
            _cdeCoreSprintf(pCdeAppIf, (char*)&rgbBuffer[0], "%02d/%02d/%04d  %02d:%02d", pTime->Month, pTime->Day, pTime->Year, pTime->Hour, pTime->Minute);
            nprintfield((char*)&rgbBuffer[0], NULL, &flags, pfnDevPutChar, (uint32_t*)&dwCount, &pDest, fWide | (32 == bIntOrLongOrInt64), 0/*is not single*/);

            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            i++;
            break;// PROCESS_TIME

        }
        case PROCESS_STATUS: {      // %r
            /*EFI_STATUS*/size_t   Status;//kgtest check size!!! = 0x5555aaaa5a96;
            char* pszErr = (int8_t*)&rgbBuffer[0];
            Status = va_arg(ap,/*EFI_STATUS*/size_t);
            _cdeCoreSprintf(pCdeAppIf, pszErr, "%qX", Status);

            #define HIGHESTBITSET (~(((size_t)-1) >> 1))

            #define EFI_SUCCESS               0
            #define EFI_LOAD_ERROR            (HIGHESTBITSET + 1)
            #define EFI_INVALID_PARAMETER     (HIGHESTBITSET + 2)
            #define EFI_UNSUPPORTED           (HIGHESTBITSET + 3)
            #define EFI_BAD_BUFFER_SIZE       (HIGHESTBITSET + 4)
            #define EFI_BUFFER_TOO_SMALL      (HIGHESTBITSET + 5)
            #define EFI_NOT_READY             (HIGHESTBITSET + 6)
            #define EFI_DEVICE_ERROR          (HIGHESTBITSET + 7)
            #define EFI_WRITE_PROTECTED       (HIGHESTBITSET + 8)
            #define EFI_OUT_OF_RESOURCES      (HIGHESTBITSET + 9)
            #define EFI_VOLUME_CORRUPTED      (HIGHESTBITSET + 10)
            #define EFI_VOLUME_FULL           (HIGHESTBITSET + 11)
            #define EFI_NO_MEDIA              (HIGHESTBITSET + 12)
            #define EFI_MEDIA_CHANGED         (HIGHESTBITSET + 13)
            #define EFI_NOT_FOUND             (HIGHESTBITSET + 14)
            #define EFI_ACCESS_DENIED         (HIGHESTBITSET + 15)
            #define EFI_NO_RESPONSE           (HIGHESTBITSET + 16)
            #define EFI_NO_MAPPING            (HIGHESTBITSET + 17)
            #define EFI_TIMEOUT               (HIGHESTBITSET + 18)
            #define EFI_NOT_STARTED           (HIGHESTBITSET + 19)
            #define EFI_ALREADY_STARTED       (HIGHESTBITSET + 20)
            #define EFI_ABORTED               (HIGHESTBITSET + 21)
            #define EFI_ICMP_ERROR            (HIGHESTBITSET + 22)
            #define EFI_TFTP_ERROR            (HIGHESTBITSET + 23)
            #define EFI_PROTOCOL_ERROR        (HIGHESTBITSET + 24)
            #define EFI_INCOMPATIBLE_VERSION  (HIGHESTBITSET + 25)
            #define EFI_SECURITY_VIOLATION    (HIGHESTBITSET + 26)
            #define EFI_CRC_ERROR             (HIGHESTBITSET + 27)
            #define EFI_END_OF_MEDIA          (HIGHESTBITSET + 28)
            #define EFI_END_OF_FILE           (HIGHESTBITSET + 31)

            #define EFI_WARN_UNKNOWN_GLYPH    1
            #define EFI_WARN_DELETE_FAILURE   2
            #define EFI_WARN_WRITE_FAILURE    3
            #define EFI_WARN_BUFFER_TOO_SMALL 4

            switch (Status) {
            case EFI_SUCCESS:               pszErr = "Success";                  break;
            case EFI_LOAD_ERROR:            pszErr = "Load Error";               break;
            case EFI_INVALID_PARAMETER:     pszErr = "Invalid Parameter";        break;
            case EFI_UNSUPPORTED:           pszErr = "Unsupported";              break;
            case EFI_BAD_BUFFER_SIZE:       pszErr = "Bad Buffer Size";          break;
            case EFI_BUFFER_TOO_SMALL:      pszErr = "Buffer Too Small";         break;
            case EFI_NOT_READY:             pszErr = "Not Ready";                break;
            case EFI_DEVICE_ERROR:          pszErr = "Device Error";             break;
            case EFI_WRITE_PROTECTED:       pszErr = "Write Protected";          break;
            case EFI_OUT_OF_RESOURCES:      pszErr = "Out of Resources";         break;
            case EFI_VOLUME_CORRUPTED:      pszErr = "Volume Corrupt";           break;
            case EFI_VOLUME_FULL:           pszErr = "Volume Full";              break;
            case EFI_NO_MEDIA:              pszErr = "No Media";                 break;
            case EFI_MEDIA_CHANGED:         pszErr = "Media changed";            break;
            case EFI_NOT_FOUND:             pszErr = "Not Found";                break;
            case EFI_ACCESS_DENIED:         pszErr = "Access Denied";            break;
            case EFI_NO_RESPONSE:           pszErr = "No Response";              break;
            case EFI_NO_MAPPING:            pszErr = "No mapping";               break;
            case EFI_TIMEOUT:               pszErr = "Time out";                 break;
            case EFI_NOT_STARTED:           pszErr = "Not started";              break;
            case EFI_ALREADY_STARTED:       pszErr = "Already started";          break;
            case EFI_ABORTED:               pszErr = "Aborted";                  break;
            case EFI_ICMP_ERROR:            pszErr = "ICMP Error";               break;
            case EFI_TFTP_ERROR:            pszErr = "TFTP Error";               break;
            case EFI_PROTOCOL_ERROR:        pszErr = "Protocol Error";           break;
            case EFI_WARN_UNKNOWN_GLYPH:    pszErr = "Warning Unknown Glyph";    break;
            case EFI_WARN_DELETE_FAILURE:   pszErr = "Warning Delete Failure";   break;
            case EFI_WARN_WRITE_FAILURE:    pszErr = "Warning Write Failure";    break;
            case EFI_WARN_BUFFER_TOO_SMALL: pszErr = "Warning Buffer Too Small"; break;
            }
            nprintfield(pszErr, NULL, &flags, pfnDevPutChar, (uint32_t*)&dwCount, &pDest, fWide | (32 == bIntOrLongOrInt64), 0/*is not single*/);
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            i++;
            break;//PROCESS_STATUS
        }
        case PROCESS_CHARSSTRING: {      // %c or %s
            short singlecharbuf[2] = { '\0','\0' };
            char* pStr = (char*)&singlecharbuf[0];
            unsigned char IsSingle = 0;
            bIntOrLongOrInt64 = (bIntOrLongOrInt64 == 0 ? 8 * sizeof(short) : bIntOrLongOrInt64);

            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case 'c':
            case 'C':   IsSingle = TRUE;
                if (32 == bIntOrLongOrInt64)
                    singlecharbuf[0] = va_arg(ap, short);
                else
                    ((char*)singlecharbuf)[0] = va_arg(ap, char);
                break;
            case 'a': bIntOrLongOrInt64 = 8 * sizeof(short);    /* in UEFI %a == ASCII STRING, in STDC float print */
            case 's':
            case 'S':   IsSingle = FALSE;
                pStr = 32 == bIntOrLongOrInt64 ? (char*)va_arg(ap, wchar_t*) : va_arg(ap, char*);
                if (NULL == pStr)
                    pStr = 32 == bIntOrLongOrInt64/* %ls format specifier */ ? (char*)L"(null)" : "(null)";
                break;
            }


            nprintfield(pStr, NULL, &flags, pfnDevPutChar, (unsigned int*)&dwCount, &pDest, 32 == bIntOrLongOrInt64, IsSingle/*is/not single*/);

            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            break;// PROCESS_CHARSSTRING
        }// PROCESS_CHARSSTRING

        case PROCESS_THE_POINTER_ITSELF:
            bIntOrLongOrInt64 = sizeof(void*) * 8;
            flags.fLZero = 0;
            flags.fPrecision = 0;
            flags.fPlus = 0;
            numdesc.nZeroExtendedLen = sizeof(void*) * 2;   //forced lenght of a pointer including leading zeros
            //numdesc.fSigned = 0;
            state = PROCESS_THE_NUMBER_ITSELF; break;
        case PROCESS_THE_NUMBER_ITSELF: {
            char* pszNum;
            ///bIntOrLongOrInt64 = (bIntOrLongOrInt64 == 0 ? 8 * sizeof(int) : bIntOrLongOrInt64);
            switch (bIntOrLongOrInt64) {
            case  0: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, int) : 0xFFFFFFFFL & (XWORD)va_arg(ap, int)); break;
            case  8: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, char) : 0x000000FFL & (XWORD)va_arg(ap, char)); break;
            case 16: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, short) : 0x0000FFFFL & (XWORD)va_arg(ap, short)); break;
            case 32: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, long) : 0xFFFFFFFFL & (XWORD)va_arg(ap, long)); break;
            case 64: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, long long) : (XWORD)va_arg(ap, long long)); break;
            }
            //numdesc.fSigned = 0;
            numdesc.fLowerCase = 1;
            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case 'd':
            case 'i': numdesc.nNumBase = 10, numdesc.fSigned = 1; break;
            case 'b': /*numdesc.nNumBase =  2*/; break;
            case 'o': numdesc.nNumBase = 8; break;
            case 'u': numdesc.nNumBase = 10; break;
            case 'x': numdesc.nNumBase = 16; break;
            case 'p':
            case 'X': numdesc.nNumBase = 16; numdesc.fLowerCase = 0; break;
            }
            //for(x = 0 ; x < sizeof(rgbBuffer) ; x++) rgbBuffer[x] = 0xFF;
            pszNum = num2str(&numdesc, fWide);
            nprintfield(pszNum, numdesc.pszSS, &flags, pfnDevPutChar, (unsigned int*)&dwCount, &pDest, fWide, 0/*is not single*/);
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            break;// PROCESS_THE_NUMBER_ITSELF
        }
        case PROCESS_COUNT: {   void* p;
            state = PROCESS_WRITECHARS;
            p = va_arg(ap, void*);
            switch (bIntOrLongOrInt64) {
            case  0: *((int*)p) = (unsigned int)(dwInitialCount - dwCount); break;
            case  8: *((char*)p) = (unsigned char)(dwInitialCount - dwCount); break;
            case 16: *((short*)p) = (unsigned short)(dwInitialCount - dwCount); break;
            case 32: *((int*)p) = (unsigned int)(dwInitialCount - dwCount); break;
            case 64: *((long long*)p) = (unsigned long long)(dwInitialCount - dwCount); break;
            }

        }break;
        case PROCESS_ERROR:
            i--;
            state = PROCESS_WRITECHARS;
            break;
        case PROCESS_PERCENT:
            if (dwCount) {
                dwCount--;
                if (pfnDevPutChar != NULL)
                    i++;
                (*pfnDevPutChar)((short)'%', &pDest);
            }
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            break;
        case PROCESS_DONT_WRITE_ANYMORE:
            dwCount = dwInitialCount + 1;   // return -1 on overflow /*kg20150210_00*/
            break;
        }//switch(state)
    }//while( ((char*)pszFormat)[i] != '\0'){

    return dwInitialCount - dwCount;
}

/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreVwxPrintf.c

Abstract:

    CDE internal worker function for the printf()-family

Author:

    Kilian Kegel

--*/
#include <cde.h>
#include <CdeServices.h>
#include <stdarg.h>
#include <stdint.h>
//
// stlib.h
//
extern __declspec(dllimport) int abs(int i);
//
// string.h
//
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
extern __declspec(dllimport) void* memcpy(void* d, const void* s, size_t n);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
//
// wchar.h
//
extern __declspec(dllimport) size_t wcslen(const wchar_t* pszBuffer);

//
// prototype
///
VWXPRINTF _cdeCoreVwxPrintf;
extern void _CdeMemPutChar(int c, void** ppDest);
extern int _cdeCoreSprintf(CDE_APP_IF* pCdeAppIf, char* pszDest, const char* pszFormat, ...);
extern void* __cdereallocCORE(CDE_APP_IF* pCdeAppIf, void* ptr, size_t size);
extern int _dsign(double d);        //https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/floating-point-primitives#_dsign-_ldsign-_fdsign

#define BUFFLEN (64 + 2)
#define XWORD unsigned long long
#define SXWORD long long
#define INTGRBCDLEN 310
#define FRACTBCDLEN 1100
#define EXPPOS 52
#define SIGPOS 63

#define STRLEN(pszStr) (fWide ? wcslen((wchar_t*)pszStr) : strlen(pszStr))
#define STRCPY(dst,src)     do{                                     \
                            char strcpy_c;                          \
                            char* strcpy_dst = dst;                 \
                            char *strcpy_src = src;                 \
                            int strcpy_i = 0;                       \
                            while (                                 \
                                strcpy_c = strcpy_src[strcpy_i],    \
                                strcpy_dst[strcpy_i++] = strcpy_c,  \
                                (char)'\0' != strcpy_c              \
                                );                                  \
                            }while (0);

typedef enum {  STRING = 1, 
                INTEGER/*integer*/, 
                FLTPTDEC/* double floating point decimal representation */, 
                FLTPTEXP/* double floating point exponential representation */ 
}num_t;

typedef struct _NUMDESC {
    IN num_t NumType;
    IN int nNumBase;
    IN short nZeroExtendedLen;
    IN unsigned char fSigned;
    IN unsigned char fLowerCase;
    IN SXWORD nNumber;
    IN char* pszNumber;
    IN int nFloatIntgrLen;      // for "abc.def" FLOATINGPOINT length of "abc"
    IN int nFloatFractLen;      // for "abc.def" FLOATINGPOINT length of "def"
    IN int nFloatExp;           // for %e/%E exponential representation w.xyzE+03
    OUT char* pszSignSharp;     //SignFieldOfDouble/Sharp
} NUMDESC;

typedef struct _FLAGS {
    unsigned int fSharp, fLZero, fLBlank, fMinus, fPlus, fPrecision, fWideStringOvr, fLZeroForNegativePrecisionAndTheNumberZero /*fPrecisionIsNegative*/;
    int nFieldsize, nPrecisionsize;//20111227 fixed wrong fieldsize if nPrecisionsize is unsigned
} FLAGS;

typedef struct _DOUBLEPRINTBUF {
    int intgrlen;                               // index into intgrbcd
    int fractlen;                               // index into fractbcd
    //char intgrbcd[INTGRBCDLEN];               // BCD of integer part
    //char fractbcd[FRACTBCDLEN];               // BCD of fraction part
    char bcd[INTGRBCDLEN + FRACTBCDLEN];        // ASCII representation w/o decimal point
    //
    //
    // | <----- INTGRBCDLEN -----> <------------------- FRACTBCDLEN -------------------> |
    //                            ^
    //                            |

    char szDbl[INTGRBCDLEN + FRACTBCDLEN + sizeof(".") + sizeof("E+123")];   // ASCII representation including decimal point
}DOUBLEPRINTBUF;

/** SignFieldOfDouble

Synopsis
    static char SignFieldOfDouble(double d);
Description
    Calculates the sign part of a double data type.

Returns
    It returns the sign bit.

    @param[in] double d

    @retval 1 - negative double d
    @retval 0 - positive double d

**/
static char SignFieldOfDouble(double d) {
    unsigned long long* pd = (void*)&d;
    return !((0LL == ((1LL << SIGPOS) & *pd)));
}

/** ExpFieldOfDouble

Synopsis
    static signed ExpFieldOfDouble(double d);
Description
    Calculates the exponent part of a double data type.

Returns
    The value returned is the true exponent of double d.
    The bias adjustment is taken from the value.

    @param[in] double d

    @retval exp

**/
static signed ExpFieldOfDouble(double d) {
    unsigned long long* pd = (void*)&d;
    unsigned mask = ((1LL << (SIGPOS - EXPPOS)) - 1);
    unsigned bias = (1LL << (SIGPOS - EXPPOS - 1)) - 1; //1023
    signed exp = (*pd >> EXPPOS);

    exp &= mask;

    return exp - bias;
}

/** FractFieldOfDouble

Synopsis
    static unsigned long long FractFieldOfDouble(double d);
Description
    Isolating the fraction part from a double data type.

Returns
    It returns the fraction bit field.

    @param[in] double d

    @retval double d[51:0]

**/
static unsigned long long FractFieldOfDouble(double d) {
    unsigned long long* pd = (void*)&d;

    return ((1LL << EXPPOS) - 1) & *pd;
}

/** IntgrBinDigits

Synopsis
    static unsigned long long IntgrBinDigits(double d);
Description
    Function calculates/shifts the fraction bitfield
    to represent the INTEGER part of a double only.
    The "hidden" bit is outlined as non-hidden.

Returns
    It returns the integer part of a double precision value.

    @param[in] double d

    @retval integer part of a double precision value

**/
static unsigned long long IntgrBinDigits(double d) {    // integer digit
    unsigned long long nRet = 0LL;
    unsigned long long fractionplushidden = FractFieldOfDouble(d) + (1LL << EXPPOS); // place hidden bit into the mantisse
    signed exp = ExpFieldOfDouble(d);
    signed shift = 0;

    exp = exp > EXPPOS ? EXPPOS : exp;

    if (exp >= 0) {
        shift = (EXPPOS - exp);
        shift = shift >= 0 ? shift : 0;
        nRet = fractionplushidden >> shift;
    }
    return nRet;
}

/** FractBinDigits

Synopsis
    static unsigned long long FractBinDigits(double d);
Description
    Function calculates/shifts the fraction bitfield
    to represent the fraction/non-integer part of a double only.
    The "hidden" bit is outlined as non-hidden.

Returns
    It returns the fraction part of a double precision value including "hidden" bit.

    @param[in] double d

    @retval fraction part of a double precision value value including "hidden" bit

**/
static unsigned long long FractBinDigits(double d) {

    unsigned long long nRet = 0LL;
    unsigned long long fractionplushidden = FractFieldOfDouble(d) + (1LL << EXPPOS); // place hidden bit into the mantisse
    signed exp = ExpFieldOfDouble(d);
    signed shift = 1 + exp;
    unsigned long long mantissemask = ((1LL << (EXPPOS + 1)) - 1);

    if (exp >= 0 && exp < EXPPOS) {
        fractionplushidden <<= shift;
        nRet = mantissemask & fractionplushidden;
    }
    else
        if (0 > exp)
            nRet = fractionplushidden;  // neg exponent doesn't contain integer part

    //
    // clear hidden bit for subnormals/denormals
    //
    if (-1023 == exp)
        nRet &= ~(1LL << EXPPOS);

    return nRet;

}

/** IntgrDigitNum

Synopsis
    static unsigned IntgrDigitNum(double d);
Description
    Function calculates the number of binary digits
    for the integer part of a double precision value.
    The "hidden" bit is considered to be non-hidden.

Returns
    It returns the number of binary digits of the integer part of a double precision value.

    @param[in] double d

    @retval number of binary digits of the integer part

**/
static unsigned IntgrDigitNum(double d) {
    signed exp = ExpFieldOfDouble(d);
    unsigned num = 0;

    if (exp >= 0) {
        exp = exp > EXPPOS ? EXPPOS : exp;
        num = exp + 1;
    }
    return num;
}

///** FractDigitNum
//
//Synopsis
//    static unsigned FractDigitNum(double d);
//Description
//    Function calculates the number of binary digits
//    for the fraction/non-integer part of a double precision value.
//    The "hidden" bit is considered to be non-hidden.
//
//Returns
//    It returns the number of binary digits of the fraction part of a double precision value.
//
//    @param[in] double d
//
//    @retval number of binary digits of the fraction part
//
//**/
//static unsigned FractDigitNum(double d) {
//    unsigned long long* pd = (void*)&d;
//    signed exp = ExpFieldOfDouble(d);
//    unsigned max = EXPPOS + 1 - IntgrDigitNum(d);
//    unsigned num, nRet = 0;
//
//    for (num = 0; num <= max; num++)
//        if (*pd & (1LL << num)) {
//            nRet = max - num;
//            break;
//        }
//
//    return nRet;
//}

/** intgr2bcd

Synopsis
    int intgr2bcd(double d, DOUBLEPRINTBUF *pDblData);
Description
    Function converts the integer part of a double precision value
    to it's corresponding BCD representation and calculates the length

Returns
    length of BCD representation in bytes

    @param[in] double d

    @retval length of BCD representation in bytes

**/
static int intgr2bcd(double d, DOUBLEPRINTBUF* pDblData) {

    //unsigned num = IntgrDigitNum(d);
    unsigned long long ll = IntgrBinDigits(d);
    int i = 0;
    signed exp = ExpFieldOfDouble(d);
    int shiftbcd = exp - EXPPOS;

    pDblData->intgrlen = 0;                     // init length at startup processing
    do {                                        // normal binary to decimal conversion
        pDblData->bcd[INTGRBCDLEN - i++] = ll % 10LL;
        ll = ll / 10LL;
    } while (0 != ll);

    pDblData->bcd[INTGRBCDLEN - i] = 0;         // terminate value behind with zero
    pDblData->intgrlen = i;                     // save length of integer digits

    while (shiftbcd-- > 0) {
        int carry = 0, digit;
        for (i = 0; i < pDblData->intgrlen; i++) {
            digit = pDblData->bcd[INTGRBCDLEN - i];
            digit <<= 1;
            digit += carry;
            if (digit > 9) {
                carry = 1;
                digit -= 10;
            }
            else
                carry = 0;
            pDblData->bcd[INTGRBCDLEN - i] = (char)digit;
        }

        if (carry) {
            pDblData->intgrlen++;
            pDblData->bcd[INTGRBCDLEN - i] = 1;
            pDblData->bcd[INTGRBCDLEN - i - 1] = 0;
        }
    }

    return pDblData->intgrlen;
}

/** fract2bcd

Synopsis
    int fract2bcd(double d, DOUBLEPRINTBUF* pDblData);
Description
    Function converts the fraction part of a double precision value
    in it's corresponding BCD representation and calculates the length

Returns
    length of BCD representation in bytes

    @param[in] double d

    @retval length of BCD representation in bytes

**/
static int fract2bcd(double d, DOUBLEPRINTBUF* pDblData) {

    unsigned long long ll = FractBinDigits(d);
    unsigned long long mask52 = (1LL << EXPPOS) - 1;
    int i = 0;
    signed exp = ExpFieldOfDouble(d);
    int shiftbcd = -1 - exp;

    //
    // adjust shift count for subnormals/denormals
    //
    if (-1023 == exp)
        shiftbcd = 1021;

    pDblData->fractlen = 0;
    pDblData->bcd[INTGRBCDLEN + 1 + 0] = 0;

    while (0 != ll) {

        ll *= 5LL;
        pDblData->bcd[INTGRBCDLEN + 1 + i++] = (char)(ll >> EXPPOS);
        ll &= mask52;
        ll <<= 1;

    };

    pDblData->bcd[INTGRBCDLEN + 1 + i] = 0;
    pDblData->fractlen = i;

    while (shiftbcd-- > 0) {
        int carry = 0, digit;
        for (i = 0; i <= pDblData->fractlen; i++) {
            digit = pDblData->bcd[INTGRBCDLEN + 1 + i] + 10 * carry;
            carry = digit & 1;
            pDblData->bcd[INTGRBCDLEN + 1 + i] = (char)(digit / 2);
        }
        pDblData->fractlen++;
        pDblData->bcd[INTGRBCDLEN + 1 + pDblData->fractlen] = 0;
    }
    return pDblData->fractlen;
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

    //fWide = 0;  // number strings are ASCII from now on

    pParms->pszNumber[i--] = '\0';

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
        pParms->pszNumber[i] = c;
        nNumber = (nNumber) / pParms->nNumBase;
        nZeroExtendedLen--;
    } while (--i && (nNumber || ((nZeroExtendedLen) > 0)));

    //
    // init sign/sharp - S/#

    STRCPY(&pParms->pszSignSharp[0], "");   // clear sign/sharp string
    if (pParms->fSigned == TRUE) {
        STRCPY(&pParms->pszSignSharp[0],fNegative == TRUE ? "-" : "+");
    }
    else {
        if (nNumberoriginal) {
            if (pParms->nNumBase == 16)
                STRCPY(&pParms->pszSignSharp[0], pParms->fLowerCase ? "0x" : "0X");
            if (pParms->nNumBase == 8)
                STRCPY(&pParms->pszSignSharp[0], "0");
        }
    }
    return &pParms->pszNumber[i + 1];
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
    NUMDESC* pNumDesc,
    FLAGS* pFlags,
    void (*pfnDevPutChar)(int, void**),
    unsigned int* pCount,
    void** ppDest,
    unsigned char fWide,
    unsigned char IsSingle/*KG20170117 Char->allow '\0' to be printed*/)
{

    int nRet = 0;
    char filler = pFlags->fLZero ? '0' : '\x20';
    char szSignSharp[3];
    int nStrlen = -1, nSSlen = -1, nFillerlen = -1, nPrecilen = -1;
    unsigned char fTrailingBlanks, fLeadingBlanks, fFillingZeros;

    STRCPY(szSignSharp, STRING == pNumDesc->NumType ? "" : pNumDesc->pszSignSharp);

    if (pFlags->fLBlank && pFlags->fPlus)   // fix "%+ d" / "% +d"
        pFlags->fLBlank = 0;                // fix "%+ d" / "% +d"
    if (pFlags->fLZero && pFlags->fMinus)   // fix "%0-2i"
        pFlags->fLZero = 0;                 // fix "%0-2i"

    if (szSignSharp[0] != '+' && szSignSharp[0] != '-') {                     // check for signed integer
        if (0 == pFlags->fSharp && szSignSharp[0] != '\0')                    // replace S/# by / if no #-flag is set
            szSignSharp[0] = '\0';                                            // (dont print 0x on hex if # is absent)!!!
    }

    pFlags->fLZero = (STRLEN(pszStr) >= (unsigned)pFlags->nFieldsize) ? 0 : pFlags->fLZero; //20111227 W/A printf("|%05d|\n",100000);

    if (szSignSharp[0] == '+') {                                                // if signed, positive number, replace '+'  by space, '0' or cut to zero string
//        pszSS[0] = (pFlags->fLBlank && !pFlags->fPlus) ? '\0' : pszSS[0];
        szSignSharp[0] = (pFlags->fLBlank && !pFlags->fLZero) ? 0x20 : szSignSharp[0];
        szSignSharp[0] = (pFlags->fLBlank &&  pFlags->fLZero) ? 0x20 : szSignSharp[0];    // fix "% 02i"
        szSignSharp[0] = (!pFlags->fLBlank && pFlags->fLZero && !pFlags->fPlus) ? '0' : szSignSharp[0];
        szSignSharp[0] = (!pFlags->fLBlank && !pFlags->fLZero && !pFlags->fPlus) ? '\0' : szSignSharp[0];
    }

    fTrailingBlanks = pFlags->fMinus ? TRUE : FALSE;
    fLeadingBlanks = !pFlags->fMinus && !pFlags->fLZero ? TRUE : FALSE;    //20111227 W/A printf("|%032.3d|\n",12); vs. printf("|%-5.0d|\n",111);
    fFillingZeros = !pFlags->fMinus && pFlags->fLZero && !pFlags->fPrecision ? TRUE : FALSE;

    // ----- W/A for precision on strings

    switch (pNumDesc->NumType) {
        case STRING: {       // have string??? (no sign/sharp string)???

        unsigned tmplen = (unsigned)STRLEN(pszStr);   // JC20170117 fixed: %c if char == '\0' doesn't print the '\0'

        if (IsSingle && '\0' == pszStr[0])   // KG20170117 fixed: %c if char == '\0' doesn't print the '\0'
            tmplen++;                       // KG20170117 fixed: %c if char == '\0' doesn't print the '\0'
        nStrlen = pFlags->fPrecision && (unsigned)pFlags->nPrecisionsize < tmplen ? pFlags->nPrecisionsize : tmplen;
        nPrecilen = 0;
        break;
    }
        case INTEGER: {
            if (pFlags->nPrecisionsize == 0 && pNumDesc->nNumber == 0) {
                STRCPY(&pszStr[0], "");  // number strings are ASCII from now on
            }
//--- 
            // For "o" conversion,  it increases the precision, if and only if necessary, to force the first digit of the result
            // to be a zero  (if  the  value  and  precision  are  both  0,  a  single  0  is  printed)
            nStrlen = 0;
            if (8 == pNumDesc->nNumBase)
                if (1 == pFlags->fSharp)
                    if (0 == pNumDesc->nNumber) {
                        if (0 == pFlags->nPrecisionsize)
                            STRCPY(&pszStr[0],"0");  // number strings are ASCII from now on
                    }//if (0 == pNumDesc->nNumber){
                    else {
                        //szSignSharp[0] = '\0';      // (dont print 0 on octal if # is absent)!!!
                    }
                    
//---
            nStrlen += (int)STRLEN(pszStr);
            if (1) {
                int OctalSharpPreciNonZeroCorrection = ((8 == pNumDesc->nNumBase) && (1 == pFlags->fSharp) && (1 < pFlags->nPrecisionsize) && (0 != pNumDesc->nNumber));

                nPrecilen = pFlags->fPrecision && pFlags->nPrecisionsize - nStrlen > 0 ? pFlags->nPrecisionsize - nStrlen - OctalSharpPreciNonZeroCorrection : 0;

            }
            //if ((8 == pNumDesc->nNumBase) && (1 == pFlags->fSharp) && (1 < pFlags->nPrecisionsize) && (0 != pNumDesc->nNumber))
            //    nPrecilen = pFlags->fPrecision && pFlags->nPrecisionsize - nStrlen > 0 ? pFlags->nPrecisionsize - nStrlen - 1 : 0;

        break;
        }

        case FLTPTDEC: {
            int offsIntgr = 0;
            int addPreciZeros = pFlags->nPrecisionsize - pNumDesc->nFloatFractLen;

            addPreciZeros = addPreciZeros < 0 ? 0 : addPreciZeros;

            nStrlen = (int)strlen(&pszStr[offsIntgr]) + 1/*_strlen(".")*/;  // INTGR.FRACT, entire possible string
            if (0 == pNumDesc->nFloatFractLen)
                nStrlen--;                                              // substract the "."
            nPrecilen = 0;                                              // precision zero are appended separately

            fFillingZeros = FALSE;
            break;
        }
    }

//    // ----- W/A for Zero precision on ZERO printf("|%5.0d|\n",0);
//    
//    if (FIXEDPOINT == pNumDesc->NumType && pFlags->fPrecision && (pFlags->nPrecisionsize == 0)) {                // have a number???
//        if (('0' == (fWide ? ((short*)pszStr)[0] : pszStr[0])) && ('\0' == (fWide ? ((short*)pszStr)[1] : pszStr[1]))) {
//            if (fWide)
//                ((short*)pszStr)[0] = '\0';
//            else
//                pszStr[0] = '\0';
//        }
//    }

    nSSlen = (int)strlen(szSignSharp);
    nFillerlen = pFlags->nFieldsize - nStrlen - nSSlen - nPrecilen < 0 ? 0 : pFlags->nFieldsize - nStrlen - nSSlen - nPrecilen;

    //
    // (L) S/# (Z/P) STR (T)
    // (Leading) S/# == 0,0x,0X,+,-,zerosize String (Zero) (Precision) STR (Trailing)
    //

        // ----- W/A for leading 0 on octals

    //if (szSignSharp[0] == '0' && szSignSharp[1] == '\0' && nPrecilen != 0) {
    //    szSignSharp[0] = '\0';            // cut string (leading '0')
    //    nFillerlen++;
    //}

    if (fLeadingBlanks  /* ###### (L) ###### */)
        nRet += nfill(0x20, nFillerlen, pCount, ppDest, pfnDevPutChar);

    if (1               /* ###### (S/#) ###### */) {
        nRet += str2dev(pfnDevPutChar, szSignSharp, pCount, ppDest, (unsigned int)-1, 0, 0);
    }

    if (fFillingZeros   /* ###### (Z) ###### */)
        nRet += nfill(filler, nFillerlen, pCount, ppDest, pfnDevPutChar);

    if (pFlags->fPrecision && pNumDesc->NumType != STRING/* ###### P ###### */)
        nRet += nfill('0', nPrecilen, pCount, ppDest, pfnDevPutChar);

    if (1               /* ###### Str ###### */) {
        if (FLTPTDEC == pNumDesc->NumType) {
            int offsIntgr = 0;
            int offsFract = pNumDesc->nFloatIntgrLen;

            nRet += str2dev(pfnDevPutChar, &pszStr[offsIntgr], pCount, ppDest, pNumDesc->nFloatIntgrLen, 0/*fWide*/, IsSingle);      // "123" part of "123.456"
            if (pNumDesc->nFloatFractLen) {
                nRet += str2dev(pfnDevPutChar, ".", pCount, ppDest, 1, 0/*fWide*/, IsSingle);                                        // "."
                nRet += str2dev(pfnDevPutChar, &pszStr[offsFract], pCount, ppDest, pNumDesc->nFloatFractLen, 0/*fWide*/, IsSingle);  // "456" part of "123.456"
            }
            //TODO: if (pFlags->fPrecision) {
            //TODO:     int diff = pFlags->nPrecisionsize - pNumDesc->nFloatFractLen - pNumDesc->nFloatIntgrLen;
            //TODO: 
            //TODO:     if (0 < diff)
            //TODO:         nRet += nfill('0', (unsigned)diff, pCount, ppDest, pfnDevPutChar);
            //TODO: }

        }
        else
            nRet += str2dev(pfnDevPutChar, pszStr, pCount, ppDest, nStrlen, fWide, IsSingle);
    }

    if (fTrailingBlanks /* ###### (T) ###### */)
        nRet += nfill(0x20, nFillerlen, pCount, ppDest, pfnDevPutChar);

    return nRet;
}

short ydes(unsigned char fDes, int i, short v) {
    return v;
}

int
_cdeCoreVwxPrintf(
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
    NUMDESC numdesc = { 
        .NumType = -1,
        .nNumBase = (unsigned char)-1,
        .nZeroExtendedLen = (short)-1,
        .fSigned = (unsigned char)-1,
        .fLowerCase = (unsigned char)-1,
        .nNumber = (SXWORD)-1,
        .pszNumber = NULL,
        .nFloatIntgrLen = 0,
        .nFloatFractLen = 0,
        .nFloatExp = -1,
        .pszSignSharp = NULL
    };
    FLAGS flags;
    unsigned char bIntOrLongOrInt64 = (unsigned char)-1;
    char rgbBuffer[sizeof(short) * BUFFLEN];
    char szSignSharp[3];               // -,+,0,0x,0X         SignFieldOfDouble/Sharp#
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
        PROCESS_THE_NUMBER,
        PROCESS_CHARSSTRING,
        PROCESS_PERCENT,
        PROCESS_WRITECHARS,
        PROCESS_COUNT,
        PROCESS_ERROR,
        PROCESS_THE_POINTER,
        PROCESS_GUID,
        PROCESS_TIME,
        PROCESS_STATUS,
        PROCESS_DONT_WRITE_ANYMORE/*kg20150210_00*/,
        PROCESS_THE_DOUBLE_G_SPECIFIER_LIBXLSXWRITER_HACK,
        PROCESS_THE_DOUBLE
    }state = PROCESS_WRITECHARS;

    DOUBLEPRINTBUF* pDblData = NULL;

    numdesc.pszNumber = rgbBuffer;
    numdesc.pszSignSharp = &szSignSharp[0];

    pszFormatW = (short*)pszFormat;
#define CHAR816(IX) fWide ? pszFormatW[IX] : ((char*)pszFormat)[IX]
    for (i = 0, c = ydes(fDes, i, CHAR816(i)); c != '\0' && (dwCount <= dwInitialCount)/*kg20150210_00*/; c = ydes(fDes, i, CHAR816(i))) {

        switch (state) {
        case PROCESS_WRITECHARS:
            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case '%':   numdesc.nNumBase = 10;
                numdesc.fSigned = FALSE;
                numdesc.nZeroExtendedLen = -1;
                numdesc.fLowerCase = 1;
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
            case 'X':   i--; state = PROCESS_THE_NUMBER; break;

            case 'p':   i--; state = PROCESS_THE_POINTER; break;

            case 'g':   if (1 == pFixParm->fUEFIFormat) {                               /* EFI_SPECIFIC*/
                            i--; numdesc.fLowerCase = 1;                                /* EFI_SPECIFIC*/
                            state = PROCESS_GUID;                                       /* EFI_SPECIFIC*/
                            break;                                                      /* EFI_SPECIFIC*/
                        }                                                               /* EFI_SPECIFIC*/
                        state = PROCESS_WRITECHARS;
                        break;

            case 'G':   if (1 == pFixParm->fUEFIFormat) {                               /* EFI_SPECIFIC*/
                            i--; numdesc.fLowerCase = 0;                                /* EFI_SPECIFIC*/
                            state = PROCESS_GUID;                                       /* EFI_SPECIFIC*/
                            break;                                                      /* EFI_SPECIFIC*/
                        }                                                               /* EFI_SPECIFIC*/
                        i--; state = PROCESS_THE_DOUBLE_G_SPECIFIER_LIBXLSXWRITER_HACK; break;
                        break;
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
        case PROCESS_PRECISION: // http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf#page=287
//TODO kg20220402            //short c;
//TODO kg20220402            // check character following the "."
//TODO kg20220402            // it is allowed to be:
//TODO kg20220402            //  1. string of decimal number 0..9
//TODO kg20220402            //      in this case the real precesion is calculated
//TODO kg20220402            //  2. optinally a length modifier h,l,j,z,t,L
//TODO kg20220402            //      int this case precision is 0
//TODO kg20220402            //  3. conversion specifier d,i,o,u,x,X f,F,e,E,g,G (c,s,p,n)???
//TODO kg20220402            //      int this case precision is 0
//TODO kg20220402            //  everthing else is just skipped...
//TODO kg20220402            switch (t = i++, c = ydes(fDes, t, CHAR816(t))) {
//TODO kg20220402            case '0':   // "." follows a decimal number
//TODO kg20220402            case '1':
//TODO kg20220402            case '2':
//TODO kg20220402            case '3':
//TODO kg20220402            case '4':
//TODO kg20220402            case '5':
//TODO kg20220402            case '6':
//TODO kg20220402            case '7':
//TODO kg20220402            case '8':
//TODO kg20220402            case '9':
//TODO kg20220402                i--;
//TODO kg20220402                state = PROCESS_FIELDSIZE;
//TODO kg20220402                break;
//TODO kg20220402
//TODO kg20220402            case 'h':   // "." follows a length modifiers
//TODO kg20220402            case 'l':
//TODO kg20220402            case 'j':
//TODO kg20220402            case 'z':
//TODO kg20220402            case 't':
//TODO kg20220402            case 'L':
//TODO kg20220402
//TODO kg20220402            case 'd':   // "." follows a conversion specifiers
//TODO kg20220402            case 'i':
//TODO kg20220402            case 'o':
//TODO kg20220402            case 'u':
//TODO kg20220402            case 'x':
//TODO kg20220402            case 'X':
//TODO kg20220402            case 'f':
//TODO kg20220402            case 'F':
//TODO kg20220402            case 'e':
//TODO kg20220402            case 'E':
//TODO kg20220402            case 'g':
//TODO kg20220402            case 'G':   *pSize = 0;
//TODO kg20220402            //case 'c': //case 's'://case 'p'://case 'n': 
//TODO kg20220402                // fall through
//TODO kg20220402            case '*':
//TODO kg20220402                i--;
//TODO kg20220402                state = PROCESS_FLAGS;
//TODO kg20220402                break;
//TODO kg20220402            default:
//TODO kg20220402                i--;
//TODO kg20220402                state = PROCESS_WRITECHARS;
//TODO kg20220402                break;
//TODO kg20220402            }
//TODO kg20220402            break;
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
            numdesc.NumType = STRING;
            nprintfield((char*)&rgbBuffer[0], &numdesc, &flags, pfnDevPutChar, (uint32_t*)&dwCount, &pDest, fWide | (32 == bIntOrLongOrInt64), 0/*is not single*/);

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
            numdesc.NumType = STRING;
            nprintfield((char*)&rgbBuffer[0], &numdesc, &flags, pfnDevPutChar, (uint32_t*)&dwCount, &pDest, fWide | (32 == bIntOrLongOrInt64), 0/*is not single*/);

            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            i++;
            break;// PROCESS_TIME

        }
        case PROCESS_STATUS: {      // %r
            /*EFI_STATUS*/size_t   Status;//kgtest check size!!! = 0x5555aaaa5a96;
            char* pszErr = (char*)&rgbBuffer[0];
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
            numdesc.NumType = STRING;
            nprintfield(pszErr, &numdesc, &flags, pfnDevPutChar, (uint32_t*)&dwCount, &pDest, fWide | (32 == bIntOrLongOrInt64), 0/*is not single*/);
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            i++;
            break;//PROCESS_STATUS
        }
        case PROCESS_CHARSSTRING: {      // %c or %s
            unsigned short singlecharbuf[2] = { '\0','\0' };
            char* pStr = (char*)&singlecharbuf[0];
            unsigned char IsSingle = 0;
            bIntOrLongOrInt64 = (bIntOrLongOrInt64 == 0 ? 8 * sizeof(short) : bIntOrLongOrInt64);
            unsigned char fSkipChr = FALSE;

            switch (t = i++, ydes(fDes, t, CHAR816(t))) {
            case 'c':
            case 'C':   IsSingle = TRUE;
                if (32 == bIntOrLongOrInt64)
                {
                    singlecharbuf[0] = va_arg(ap, unsigned short);

                    if(0 == pFixParm->fWide)                            // with "narrow" printf()...
                        if (singlecharbuf[0] > 255)                     //
                            fSkipChr = TRUE;                            // ...skip wide char > 255
                }
                else {
                    if (0 == pFixParm->fWide)
                        ((char*)singlecharbuf)[0] = va_arg(ap, char);
                    else
                        singlecharbuf[0] = va_arg(ap, unsigned short);
                }
                break;
            case 'a': if (1 == pFixParm->fUEFIFormat) {             /* EFI_SPECIFIC*/
                bIntOrLongOrInt64 = 8 * sizeof(short);  /* EFI_SPECIFIC*/   /* in UEFI %a == ASCII STRING, in STDC float print */
            }                                           /* EFI_SPECIFIC*/
                    else
                break;

            case 's':
            case 'S':   IsSingle = FALSE;
                pStr = 32 == bIntOrLongOrInt64 ? (char*)va_arg(ap, wchar_t*) : va_arg(ap, char*);
                if (NULL == pStr)
                    pStr = 32 == bIntOrLongOrInt64/* %ls format specifier */ ? (char*)L"(null)" : "(null)";
                break;
            }

            if (FALSE == fSkipChr)                                   // skip wide char > 255
            {
                numdesc.NumType = STRING;
                nprintfield(pStr, &numdesc, &flags, pfnDevPutChar, (unsigned int*)&dwCount, &pDest, (32 == bIntOrLongOrInt64), IsSingle/*is/not single*/);
            }
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            break;// PROCESS_CHARSSTRING
        }// PROCESS_CHARSSTRING

        case PROCESS_THE_POINTER:
            bIntOrLongOrInt64 = sizeof(void*) * 8;
            flags.fLZero = 0;
            flags.fPrecision = 0;
            flags.fPlus = 0;
            numdesc.nZeroExtendedLen = sizeof(void*) * 2;   //forced length of a pointer including leading zeros
            //numdesc.fSigned = 0;
            state = PROCESS_THE_NUMBER; break;
        case PROCESS_THE_DOUBLE_G_SPECIFIER_LIBXLSXWRITER_HACK: {
            double d;
            int cIntgr, cFract/*count*/;
            char* pszDbl, * pBCD;
            int nPrecisionsize = 0 == flags.fPrecision ? 6 : (flags.nPrecisionsize > FRACTBCDLEN ? FRACTBCDLEN : flags.nPrecisionsize/*range check*/);    // set default precision size

            //
            // allocate the double print buffer for processing (1418 bytes)
            //
            if (NULL == pDblData)
            {
                pDblData = pCdeAppIf->pCdeServices->pMemRealloc(
                    pCdeAppIf,
                    NULL,                       // IN void *ptr,   /* input pointer for realloc */
                    sizeof(DOUBLEPRINTBUF),     // IN size_t size, /*  input size for realloc   */
                    &pCdeAppIf->pCdeServices->HeapStart
                );
            }
            memset(&pDblData->bcd, 0xff, INTGRBCDLEN + FRACTBCDLEN);

            d = va_arg(ap, double);

            cIntgr = intgr2bcd(d, pDblData);                                    // convert INTGR part to BCD
            cFract = fract2bcd(d, pDblData);                                    // convert FRACT part to BCD
            
            //
            // cut/expand precision
            //
            if (cFract > nPrecisionsize)
                cFract = nPrecisionsize;
            else
                memset(&pDblData->bcd[INTGRBCDLEN + cFract + 1], 0, 1 + nPrecisionsize - cFract), // fill with 0xff
                cFract = nPrecisionsize;

            if (1)
            {
                int carry = 0 /*rounding carry*/;
                int startIntgrASCII/*depending on rounding result 99.xxx -> 100.yyy */;
                char* pBcdLsdFract = &pDblData->bcd[INTGRBCDLEN + cFract];  // end of FRACT part Least Significant Digit
                char* pBcdMsdIntgr = &pDblData->bcd[INTGRBCDLEN - cIntgr];      // begin of INTGR part Most Significant Digit

                //
                // make it BCD to ASCII
                //
                for (pBCD = pBcdLsdFract; pBCD >= pBcdMsdIntgr; pBCD--) {
                    int val;
                    val = pBCD[0] + carry;
                    carry = val == 9 + 1;
                    if (carry)
                        val = 0;
                    pBCD[0] = (char)(val + '0');
                }
                
                startIntgrASCII = '0' == pBcdMsdIntgr[0] ? 1 : 0;                               // offset of start of string
                pBcdMsdIntgr = &pBcdMsdIntgr[startIntgrASCII];                                  // start of string

                numdesc.nFloatIntgrLen = cIntgr + 1 - startIntgrASCII;                          // strlen of INTGR part
                numdesc.nFloatFractLen = (int)(&pBcdLsdFract[0] - &pDblData->bcd[INTGRBCDLEN]);
                numdesc.NumType = FLTPTDEC;

                pszDbl = pBcdMsdIntgr;

                //
                // W/A for Excel 2016, convert "0.000000000000000" to "0"
                //
                if (0.0 == d)
                    numdesc.nFloatFractLen = 0;;
            }
            memset(&numdesc.pszSignSharp[0], 0, 3);

            numdesc.pszSignSharp[0] = _dsign(d) ? '-' : '+';                                    // check sign of double d

            nprintfield(pszDbl, &numdesc, &flags, pfnDevPutChar, (unsigned int*)&dwCount, &pDest, 0/*fWide*/, 0/*is not single*/);
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;          //set state before decrement dwCount    /*kg20150210_00*/
            i++;
            break;
        }
        case PROCESS_THE_NUMBER: {
            char* pszNum;
            ///bIntOrLongOrInt64 = (bIntOrLongOrInt64 == 0 ? 8 * sizeof(int) : bIntOrLongOrInt64);
            switch (bIntOrLongOrInt64) {
            case  0: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, int) : 0xFFFFFFFFL & (XWORD)va_arg(ap, int)); break;
            case  8: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, char) : 0x000000FFL & (XWORD)va_arg(ap, char)); break;
            case 16: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, short) : 0x0000FFFFL & (XWORD)va_arg(ap, short)); break;
            case 32: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, long) : 0xFFFFFFFFL & (XWORD)va_arg(ap, long)); break;
            case 64: numdesc.nNumber = (numdesc.fSigned ? (SXWORD)va_arg(ap, long long) : (XWORD)va_arg(ap, long long)); break;
            }
            
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
            numdesc.NumType = INTEGER;
            pszNum = num2str(&numdesc, 0 /*fWide*/);
            nprintfield(pszNum, &numdesc, &flags, pfnDevPutChar, (unsigned int*)&dwCount, &pDest, 0/*fWide*/, 0/*is not single*/);
            state = dwCount ? PROCESS_WRITECHARS : PROCESS_DONT_WRITE_ANYMORE;  //set state before decrement dwCount    /*kg20150210_00*/
            break;// PROCESS_THE_NUMBER
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
    
    //
    // free the floating point data buffer
    //
    if(NULL != pDblData)
        pCdeAppIf->pCdeServices->pMemRealloc(pCdeAppIf, pDblData, 0, &pCdeAppIf->pCdeServices->HeapStart);

    return dwInitialCount - dwCount;
}

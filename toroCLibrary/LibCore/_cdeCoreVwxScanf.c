/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeCoreVwxScanf.c

Abstract:

    CDE internal worker function for the scanf()-family

Author:

    Kilian Kegel

--*/
//20181222 TODO: add %l[]
//20181222 TODO: %S %C
//20161126 TODO: add %p
#include <CdeServices.h>
#include <stdio.h>
#include <limits.h>
//
// ctype.h
//
extern __declspec(dllimport) int isspace(int c);
extern __declspec(dllimport) int isdigit(int c);
extern __declspec(dllimport) int isxdigit(int c);
//
// string.h
//
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);

#include <stdarg.h>

#define XWORD unsigned long long
#define BUFFLEN (64 + 2)

#define WSPC 0x20       /*white space*/
#define DNUM 9          /*decimal number*/
#define EARLY_MATCHING_FAILURE -2

#undef void
#define void unsigned char

extern int ComputeFieldSize(char* pfieldsizebuf);
extern uint64_t _cdeUMUL128(uint64_t a, uint64_t b, uint64_t* pHigh);

static int StoreTerminate(int fWide, int fTerminate, void* pVoid, int nMaxChars, int* pIndex, int c) {
    int fWritten = 0;
    if (NULL != pVoid) {
        if (*pIndex < nMaxChars) {
            if (fWide == 1) {
                (((wchar_t*)pVoid)[(*pIndex)++] = (wchar_t)c);
                if (fTerminate)
                    (((wchar_t*)pVoid)[*pIndex] = (wchar_t)'\0');
            }
            else {
                (((char*)pVoid)[(*pIndex)++] = (char)c);
                if (fTerminate)
                    (((char*)pVoid)[*pIndex] = (char)'\0');
            }
            fWritten = 1;
        }
    }
    else {
        //
        // NOTE: do _NOT_ update pIndex. "fWritten == 1 && *pIndex == 0" identifies a skipped token!!!
        //
        if (*pIndex < nMaxChars)
            fWritten = 1;
    }
    return fWritten;
}

static int inrange(int l, wchar_t begin, wchar_t end) {
    wchar_t tmp = end;

    if (begin > end) {
        end = begin;
        begin = tmp;
    }

    return (begin <= (wchar_t)l && (wchar_t)l <= end);
}

static int GetCharAndCountUp(IN int* pCnt,
    IN int (*pfnDevGetChar)(void** ppSrc),
    IN void** ppSrc,
    IN int EndMarker/* 0 / EOF */)
{
    int c;

    c = (*pfnDevGetChar)(ppSrc);

    *pCnt += (c == EndMarker ? 0 : 1);

    return c;
}

static int UngetCharAndCountDown(
    int c, int* pCnt,
    IN int (*pfnDevUngetChar)(int c, void** ppDst),
    IN void** ppDst,
    IN int EndMarker/* 0 / EOF */) {
    c = (*pfnDevUngetChar)(c, ppDst);
    *pCnt -= (c == EndMarker ? 0 : 1);
    return c;
}

static unsigned char IsCharInNumberRange(int nNumBase, int c) {
    unsigned char fRet = FALSE;

    switch (nNumBase) {
    case 0:/*autodetection for the first char only as long as nNumBase == 0, can be 0->OCTAL, or 1..9 for DECIMAL*/
        if (c >= '0' && c <= '9')
            fRet = TRUE;
        break;
    default:
        if ((c >= '0' && c <= ('0' + (nNumBase > 10 ? 10 : nNumBase) - 1)) ||
            (c >= 'A' && c <= ('A' + (nNumBase > 10 ? nNumBase - 1 - 10 : -1/* always false*/))) ||
            (c >= 'a' && c <= ('a' + (nNumBase > 10 ? nNumBase - 1 - 10 : -1/* always false*/))))
            fRet = TRUE;
        break;
    }
    return fRet;
}

static unsigned char IsSeparator(char c) {

    return (c == 0x20 || c == '\t' || c == '\n' || c == '\0');
}

typedef struct _STRDESC {
    int     nWidth;                             // number of bytes to store result -> 1,2,4
    unsigned char fSigned;                           // signed %d or unsigned %x etc.
    unsigned char fAsterix;                          // asterisk
//    wchar_t (*pfnDevGetChar)(void **ppSrc);
    XWORD   xwNumber;
    int   nNumBase;
    int     EndMarker;
}STRDESC;

static int str2num(STRDESC* pParms, int (*pfnDevGetChar)(void** ppSrc), int (*pfnDevUngetChar)(int u, void** ppSrc), int* pOverallCount, IN void** ppSrc, int EndMarker) {
    int i;
    int c = -1;
    unsigned char fNegativ = 0; //positive
    unsigned char fBaseAuto/*detect*/ = pParms->nNumBase == 0;//base autodetction if pParms->nNumBase == 0
    int nMaxChars = pParms->nWidth;
    int nCharsProcessed = 0;
    int nTokenScanned = EOF;
    int nHexIDPecedingZeros = 0;

    char rgBuf[BUFFLEN];
    enum STATE {
        PROCESS_BLANKS,
        PROCESS_SIGN,
        PROCESS_NUMBER,
        PROCESS_THE_END,
        PROCESS_DONE,
        PROCESS_ZEROX,
        PROCESS_ERROR
    }state = PROCESS_BLANKS;

    memset(rgBuf, -1, sizeof(rgBuf));
    i = 0;
    while (state != PROCESS_DONE) {

        if (state != PROCESS_THE_END) {
            c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, ppSrc, EndMarker);
        }
        switch (state) {
        case PROCESS_BLANKS:
            if (c == EndMarker)
            {
                state = PROCESS_DONE; 
                break;
            }

            switch (c) {
            //case '\0':
            //case  EOF: state = PROCESS_DONE; break;
            case    0: nTokenScanned = EARLY_MATCHING_FAILURE; 
                       state = PROCESS_DONE;    // '\x20','\0' found when reading
                                                // blanks from wcs-style input
                break;
            case 0x20:
            case '\r':
            case '\v':
            case '\f':
            case '\n':
            case '\t':  break;
            case '-':
            case '+':  fNegativ = (c == '+' ? 0 : 1);
                nMaxChars = nMaxChars ? nMaxChars - 1 : 0;
                state = PROCESS_SIGN; break;
            default:    if (IsCharInNumberRange(pParms->nNumBase, c)) {
                rgBuf[i++] = (char)(c <= '9' ? c : (~0x20 & c) - 1 - 6) - '0';
                nTokenScanned = 1;
                nMaxChars = nMaxChars ? nMaxChars - 1 : 0;
                nHexIDPecedingZeros += (c == '0' ? 1 : 0);
                state = PROCESS_NUMBER;
            } else {
                UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, ppSrc, EndMarker);
                nTokenScanned = '\0' == c ? 0 : -1;
                state = PROCESS_DONE;
            }break;
            }break;
        case PROCESS_SIGN:
            do {
                if (!nMaxChars) {
                    nTokenScanned = 0;
                    state = PROCESS_DONE;
                    break;//do ..while(0)
                }

                if (c == pParms->EndMarker) { nTokenScanned = -1; state = PROCESS_DONE; break; }//do ..while(0)

                if (IsCharInNumberRange(pParms->nNumBase, c)) {
                    rgBuf[i++] = (char)(c <= '9' ? c : (~0x20 & c) - 1 - 6) - '0';
                    nTokenScanned = 1;
                    nMaxChars = nMaxChars ? nMaxChars - 1 : 0;
                    nHexIDPecedingZeros += (c == '0' ? 1 : 0);
                    state = PROCESS_NUMBER;
                }
                else {
                    UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, ppSrc, EndMarker);
                    nTokenScanned = 0;
                    state = PROCESS_DONE;
                }
                nMaxChars = nMaxChars ? nMaxChars - 1 : 0;
            } while (0);
            break;//PROCESS_SIGN

        case PROCESS_NUMBER:
            if (fBaseAuto && (nHexIDPecedingZeros == 1) && (pParms->nNumBase == 0)) {// if autodetection AND leading 0 assume OCTAL, until 'X' -> HEX
                pParms->nNumBase = 8;
            }
            else
                pParms->nNumBase = (pParms->nNumBase == 0 ? pParms->nNumBase = 10 : pParms->nNumBase);// if autodetection AND NOT leading 0 assume DECIMAL

            if (IsCharInNumberRange(pParms->nNumBase, c)) {
                rgBuf[i++] = (char)(c <= '9' ? c : (~0x20 & c) - 1 - 6) - '0';
                nMaxChars = nMaxChars ? nMaxChars - 1 : 0;
                //state = PROCESS_NUMBER;
            }
            else
                if (fBaseAuto && (pParms->nNumBase == 8) && (nHexIDPecedingZeros == 1) && (i == 1 /*offset where expected*/) && (c == 'x' || c == 'X')) {
                    //if(fBaseAuto && (nHexIDPecedingZeros == 1) && (pParms->nNumBase == 8)){// if autodetection AND leading 0 assume OCTAL, until 'X' -> HEX
                    pParms->nNumBase = 16;
                    fBaseAuto = FALSE;
                    state = PROCESS_ZEROX;
                    //}
                }
                else
                    if (!fBaseAuto && (pParms->nNumBase == 16) && (nHexIDPecedingZeros == 1) && (i == 1 /*offset where expected*/) && (c == 'x' || c == 'X')) {
                        // also accept preceeding 0x/0X if numbase is predifined as hexadecimal
                        state = PROCESS_ZEROX;
                    }
                    else
                    {
                        UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, ppSrc, EndMarker);
                        state = PROCESS_THE_END;
                    }
            break;
        case PROCESS_THE_END: {
            XWORD xwWeight = 1, xwNumber = 0, xwNumOld, xwSummand;
            char fOvl = 0;

            while (--i != -1 && 0 == fOvl) 
            {
                uint64_t qwOvl = 0;

                xwSummand = _cdeUMUL128/*_umul128*/(xwWeight, (uint64_t)rgBuf[i], &qwOvl);
                if (0LL != qwOvl)
                    fOvl |= 1;

                xwNumOld = xwNumber;
                xwNumber += xwSummand;
                if(xwNumber < xwNumOld)
                    fOvl |= 1;

                if (0 != i)
                {
                    xwWeight = _cdeUMUL128/*_umul128*/(xwWeight, (uint64_t)pParms->nNumBase, &qwOvl);
                    if (0LL != qwOvl)
                        fOvl |= 1;
                }
            }
            //
            // signed %d / %i can not be greater then LLONG_MAX and not be smaller then LLONG_MIN, if signed, limit to LLONG_MAX / LLONG_MIN
            //
            //xwNumber = pParms->fSigned ? (fOvl || (xwNumber > LLONG_MAX) ? (fNegativ ? LLONG_MIN : LLONG_MAX) : xwNumber) : (xwNumber);
            if (pParms->fSigned)
            {
                if (fOvl || (xwNumber > LLONG_MAX))
                {
                    if (fNegativ)
                        xwNumber = (XWORD)LLONG_MIN;
                    else
                        xwNumber = (XWORD)LLONG_MAX;
                }
            }
            else {
                if (fOvl)
                    xwNumber = ULLONG_MAX;
            }
            
            pParms->xwNumber = (fNegativ && 0 == fOvl) ? -((long long)xwNumber) : xwNumber; //sign

            state = PROCESS_DONE;
            break;
        }
        case PROCESS_ZEROX:
            state = (((pParms->nNumBase == 16) && (nHexIDPecedingZeros == 1) && (isxdigit(c))) ? PROCESS_NUMBER : PROCESS_THE_END);
            UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, ppSrc, EndMarker);
            if (state == PROCESS_THE_END) {
                UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, ppSrc, EndMarker);//TODO: verify!!!
                nTokenScanned = 0;
            }
            break;
        }
        if (state != PROCESS_DONE)
            state = nMaxChars ? state : PROCESS_THE_END;
    }//while
    pOverallCount = (nTokenScanned < 1 ? pOverallCount : pOverallCount + nCharsProcessed);
    return nTokenScanned;
}

int
_cdeCoreVwxScanf(
    CDE_APP_IF* pCdeAppIf,
    IN ROMPARM_VWXSCANF* pFixParm,
    IN const char* pszFormat,
    IN int (*pfnDevGetChar)(void** ppSrc),
    IN int (*pfnDevUngetChar)(int c, void** ppDst),
    IN unsigned char* pSrc,
    IN unsigned int dwCount,
    OUT unsigned char** ppEnd,// EndPtr for strtol() and strtoul()
    IN va_list ap
) {


    unsigned char fWide = pFixParm->fWide;
    STRDESC stParms = { -1,(unsigned char)-1,(unsigned char)-1,(XWORD)-1,(unsigned char)-1,-1 };
    void* pVoid = NULL;
    int i = 0, nTmp, * pInt, c = -1/*char from input device*/, e = -1/*previous char from the format string*/, format = -1/*char from the format string*/, formctype/*ctype*/ = -1, g = -1;
    char fieldsizebuf[8];
    int  ifieldsizebuf = -1;
    int nTokenScanned = 0;
    int nCharsProcessed;
    wchar_t* pszFormatW = (wchar_t*)pszFormat;
    int OverallCount = 0, * pOverallCount = &OverallCount;
    int* pWidth = &stParms.nWidth;
    int nMaxChars;
    unsigned char* pSrcOriginal = pSrc; // memorize for error handling 

    int len = 0, iSSstart = 0/*i at scanset start*/, o = 0/* output index to destination pointer*/;
    void* pSSRngStart = NULL/*range start*/, * pSSRngEnd = NULL/*range end*/;        // used together with '-' operator in scan sets
    unsigned char fSSToken = 0, fInside = 0, fInverted = 0;


    enum DSTSIZE {
        DSTDFLT,                /*0 == destination default  */
        DST8 = sizeof(char),    /*always  8 Bit             */
        DST16 = sizeof(short),  /*always 16 Bit             */
        DST32 = sizeof(int),    /*always 32 Bit             */
        DST64 = sizeof(long long)
    }dstsize = DSTDFLT; // integersize 16/32/64
    enum STATE {
        PROCESS_FORMATSTR,
        PROCESS_GETCHECKEDINPUT,
        PROCESS_GETINPUT,
        PROCESS_FLAGS,
        PROCESS_BASE,
        PROCESS_WIDTH,
        PROCESS_TYPELEN,
        PROCESS_CHAR,
        PROCESS_STRING,
        PROCESS_INT,
        PROCESS_COUNT,
        PROCESS_SCANSET_BEGIN_CKINV,
        PROCESS_SCANSET_BEGIN_CKLEN,
        PROCESS_SCANSET_RANGEORCHAR,
        PROCESS_SCANSET_SCAN,
        PROCESS_SCANSET_END,
        PROCESS_END = 0x8000,
        PROCESS_ERROR,
        PROCESS_EXIT
    }state = PROCESS_FORMATSTR;

    for (i = 0; state != PROCESS_EXIT; ) {

        if (!(state & PROCESS_END)) {   // if NOT  PROCESS_END and successors... (PROCESS_ERROR)

            e = (i == 0 ? -1 : format);

            format = fWide ? pszFormatW[i++] : pszFormat[i++];
            formctype = isdigit(format) ? DNUM : (isspace(format) ? WSPC : format);

            if (i != 0)
                g = fWide ? pszFormatW[i] : pszFormat[i];

            state = ('\0' == format ? PROCESS_END : state);

        }

        switch (state) {
            //
            // scanset implementation %[...]
            //
        case PROCESS_SCANSET_BEGIN_CKINV:

            state = PROCESS_SCANSET_BEGIN_CKLEN;

            fInverted = ('^' == format);          // check for '^' at the very beginning
            if (0 == fInverted)
                i--;                        // adjust i against autoincrement if it wasn't '^'
            iSSstart = i;

            pVoid = stParms.fAsterix ? NULL : va_arg(ap, void*); // load output pointer dont get and use on asterisk
            nMaxChars = stParms.nWidth;
            nCharsProcessed = 0;
            o = 0;
            fInside = 0;
            fSSToken = 0;
            break;

        case PROCESS_SCANSET_BEGIN_CKLEN:

            state = PROCESS_SCANSET_RANGEORCHAR;

            len = ']' == format ? 1 : 0;                                                     // check for ']' character at the very beginning of the scanset
            while (']' != (fWide ? pszFormatW[i - 1 + len] : pszFormat[i - 1 + len])) {     // check terminating ']' character
                if ('\0' == (fWide ? pszFormatW[i - 1 + len] : pszFormat[i - 1 + len])) {   // check for wrong format string (missing ']' at the end)
                    state = PROCESS_ERROR;
                    break;//while
                }
                len++;
            }
            i--;                        // adjust i against autoincrement, keep at begin of scanset
            break;

        case PROCESS_SCANSET_RANGEORCHAR: {

            if (i - 1 - iSSstart == len) {

                if (fInverted) {
                    if (!fInside) {
                        c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker);
                        if (c == pFixParm->EndMarker) {
                            state = PROCESS_SCANSET_END;
                            break;
                        }
                        if (StoreTerminate(fWide, 1/*fTerminate*/, pVoid, *pWidth, &o, c)) {
                            if(0 != o)
                            fSSToken = 1;
                        }
                        else {
                            UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
                            i--;
                            state = PROCESS_SCANSET_END;
                            break;
                        }
                        fInside = 0;
                    }
                    else {
                        //UngetCharAndCountDown(c,&nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
                        i--;
                        state = PROCESS_SCANSET_END;
                        break;
                    }
                }
                else {
                    if (!fInside) {
                        i--;
                        state = PROCESS_SCANSET_END;
                        break;
                    }
                }
                i = iSSstart;          // restart scanset
            }
            else {
                i--;
            }

            if (i != iSSstart && '-' == format && ']' != e) {   // if '-' appears at the very beginning of the scan set, than it is used as character, not as range operator
                                                                // also consider that ']' at the very beginning of the scanset DOES NOT open a scan range!!!
                                                                // MSFTBUG/ANOMALIE
                fWide ? (pSSRngStart = (void*)&pszFormatW[i - 1]) : (pSSRngStart = (void*)&pszFormat[i - 1]);
                fWide ? (pSSRngEnd = (void*)&pszFormatW[i + 1]) : (pSSRngEnd = (void*)&pszFormat[i + 1]);
            }
            else {
                fWide ? (pSSRngStart = (void*)&pszFormatW[i - 0]) : (pSSRngStart = (void*)&pszFormat[i - 0]);
                fWide ? (pSSRngEnd = (void*)&pszFormatW[i - 0]) : (pSSRngEnd = (void*)&pszFormat[i - 0]);
            }

            state = PROCESS_SCANSET_SCAN;

            break;
        }
        case PROCESS_SCANSET_SCAN:

            c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker);
            if (c == pFixParm->EndMarker) {
                state = PROCESS_SCANSET_END;
            }
            else {
                if (inrange(c, fWide ? *((wchar_t*)pSSRngStart) : *((char*)pSSRngStart), fWide ? *((wchar_t*)pSSRngEnd) : *((char*)pSSRngEnd)))
                {
                    fInside = 1;
                    if (!fInverted) {
                        if (StoreTerminate(fWide, 1/*fTerminate*/, pVoid, *pWidth, &o, c)) {
                            if (0 != o)
                            fSSToken = 1;
                        }
                        else {
                            UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
                            i--;
                            state = PROCESS_SCANSET_END;
                            break;
                        }
                        i = iSSstart;          // restart scanset
                        fInside = 0;
                    }
                    else {
                        UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);

                    }
                }
                else {
                    if (!fInverted) {
                        if (i - 1 - iSSstart == len) {
                            i--;
                            state = PROCESS_SCANSET_END;
                        }
                        else {
                            UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
                        }
                    }
                    else {
                        UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
                    }
                }
            }
            state = state == PROCESS_SCANSET_END ? PROCESS_SCANSET_END : PROCESS_SCANSET_RANGEORCHAR;
            break;

        case PROCESS_SCANSET_END:
            if (fSSToken)
                nTokenScanned++;
            i = iSSstart + len + 1;
            state = (0 == fSSToken ? PROCESS_END : PROCESS_FORMATSTR);
            break;

        case PROCESS_FORMATSTR:
            switch (formctype) {
            case '%': stParms.fSigned = 0;
                stParms.fAsterix = 0;
                stParms.nNumBase = 10;
                stParms.nWidth = INT_MAX;
                stParms.EndMarker = pFixParm->EndMarker;
                for (ifieldsizebuf = 0; ifieldsizebuf < 8; ifieldsizebuf++)fieldsizebuf[ifieldsizebuf] = -1;
                ifieldsizebuf = 0;
                dstsize = DSTDFLT;
                pWidth = &stParms.nWidth;
                state = PROCESS_FLAGS; break;
            case WSPC: i--; state = PROCESS_GETINPUT; break;
            default: i--; state = PROCESS_GETCHECKEDINPUT; break;
            }break;
        case PROCESS_GETCHECKEDINPUT:
            nCharsProcessed = 0;
            c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker);
            state = (format == c ? PROCESS_FORMATSTR : PROCESS_END);
            if (state == PROCESS_END)
                UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
            OverallCount += nCharsProcessed;
            break;
        case PROCESS_GETINPUT:
            nCharsProcessed = 0;
            switch (formctype) {
            case WSPC: do {
                c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker);
            } while (isspace(c));
            UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
            OverallCount += nCharsProcessed;
            state = PROCESS_FORMATSTR; break;
            default: state = (c == GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker)) ? PROCESS_FORMATSTR : PROCESS_ERROR;
                OverallCount += nCharsProcessed;               // adjust overall count
                break;
            }break;
        case PROCESS_FLAGS:
            switch (formctype) {
            case '`':   pWidth = &stParms.nNumBase;
                state = PROCESS_BASE;
                break;

            case DNUM:  state = PROCESS_WIDTH; i--; break;

            case 'j':
            case 't':
            case 'z':
            case 'I':
            case 'h':
            case 'l':
            case 'q':
            case 'L':   state = PROCESS_TYPELEN; i--; break;
            case 'n':   state = PROCESS_COUNT; i--; break;
            case '*':   stParms.fAsterix = 1; break;
                //todo: add uppercase C + S     case 'C':
                //todo: add uppercase C + S     case 'S':
            case 'c':   state = PROCESS_CHAR; i--; break;
            case 's':   state = PROCESS_STRING; i--; break;

            case 'd': case 'i': case 'b': case 'B': case 'o': case 'u': case 'x': case 'p':
            case 'X':   i--; state = PROCESS_INT; break;
            case '%':   i--; state = PROCESS_END; break;
            case '[':   state = PROCESS_SCANSET_BEGIN_CKINV; break;
            default:   state = PROCESS_ERROR; break;
            }break;
        case PROCESS_TYPELEN:
            switch (formctype) {
                //todo: add I32 + I64 support to be MS compatible
            case 'h':   dstsize = DST16;
                if ('h' == (fWide ? pszFormatW[i] : ((char*)pszFormat)[i])) {
                    dstsize = DST8;
                    i++;
                }
                break;
            case 'l':   dstsize = DST32;
                if ('l' == (fWide ? pszFormatW[i] : ((char*)pszFormat)[i])) {
                    dstsize = DST64;
                    i++;
                }
                break;

            case 'q':
            case 'L':   dstsize = DST64; break;
            case 'I':   /* I32, I64 == ms specific */
                if ('3' == (fWide ? pszFormatW[i] : ((char*)pszFormat)[i]) &&
                    '2' == (fWide ? pszFormatW[i + 1] : ((char*)pszFormat)[i + 1])) {
                    dstsize = DST32;
                    i += 2;
                }
                else
                    if ('6' == (fWide ? pszFormatW[i] : ((char*)pszFormat)[i]) &&
                        '4' == (fWide ? pszFormatW[i + 1] : ((char*)pszFormat)[i + 1])) {
                        dstsize = DST64;
                        i += 2;
                    }
                    else {
                        i--; state = PROCESS_ERROR;
                    }
                break;
            case 'j':   dstsize = DST64; break;
            case 't':   dstsize = sizeof(ptrdiff_t); break;
            case 'z':   dstsize = sizeof(size_t); break;

            case DNUM:  i--; state = PROCESS_WIDTH; break;
            default:   i--; state = PROCESS_FLAGS; break;
            }break;
        case PROCESS_BASE:
        case PROCESS_WIDTH:
            switch (formctype) {
            case DNUM:  fieldsizebuf[ifieldsizebuf++] = (char)(format)-'0';
                if (ifieldsizebuf > 7)
                    state = PROCESS_ERROR;
                break;
                //todo: add I32 + I64 support to be MS compatible
            case 'h':
            case 'l':
            case 'q':
            case 'L':   i--; *pWidth = ComputeFieldSize(&fieldsizebuf[0]); state = PROCESS_TYPELEN;
                break;
            default:    i--; *pWidth = ComputeFieldSize(&fieldsizebuf[0]); state = PROCESS_FLAGS;
                break;
            }break;

        case PROCESS_STRING: {
            int fCharProcessed = 0;
            nMaxChars = stParms.nWidth;
            nCharsProcessed = 0;
            o = 0;  // output index
            pVoid = stParms.fAsterix ? NULL : va_arg(ap, void*); // load output pointer dont get and use on asterisk
            
            if (DST8 == dstsize)        // check for "%hhs" that IS NOT supported
            {                           // instead %hs, %ls and %lls ARE supported instead
                state = PROCESS_END;
                break;
            }

            while (nMaxChars) {
                c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker);
                if (c == pFixParm->EndMarker) {
                    state = PROCESS_END;
                    break;//while(nMaxChars--)
                }

                if (isspace(c) || (c == pFixParm->EndMarker)) {
                    if (fCharProcessed) {
                        UngetCharAndCountDown(c, &nCharsProcessed, pfnDevUngetChar, &pSrc, pFixParm->EndMarker);
                        break;
                    }
                }
                else {
                    unsigned char fStrWidth = DSTDFLT == dstsize ? fWide : DST16 != dstsize/*%hs == 16 -> fStrWidth = 0*/;

                    fCharProcessed |= StoreTerminate(fStrWidth, 1/*fTerminate*/, pVoid, INT_MAX, &o, c);
                    nMaxChars--;
                }
            }

            if (fCharProcessed && 0 != o)
                nTokenScanned++;

            OverallCount += nCharsProcessed;
            state = (state == PROCESS_END ? PROCESS_END : (0 == fCharProcessed ? PROCESS_END : PROCESS_FORMATSTR));
            break;
        }
        case PROCESS_CHAR: {
            int fCharProcessed = 0;
            //__debugbreak();
            nMaxChars = stParms.nWidth;
            nCharsProcessed = 0;
            o = 0;  // output index
            pVoid = stParms.fAsterix ? NULL : va_arg(ap, void*); // load output pointer dont get and use on asterisk
            nMaxChars = (INT_MAX == nMaxChars) ? 1 : nMaxChars;
            while (nMaxChars--) {
                c = GetCharAndCountUp(&nCharsProcessed, pfnDevGetChar, &pSrc, pFixParm->EndMarker);
                if (c == pFixParm->EndMarker) {
                    state = PROCESS_END;
                    break;//while(nMaxChars--)
                }
                fCharProcessed |= StoreTerminate(fWide, 0/*fTerminate*/, pVoid, INT_MAX, &o, c);
            }
            OverallCount += nCharsProcessed;
            if (fCharProcessed && 0 != o)
                nTokenScanned++;
            state = (state == PROCESS_END ? PROCESS_END : (0 == fCharProcessed ? PROCESS_END : PROCESS_FORMATSTR));
            break;
        }
        case PROCESS_INT:
            switch (format) {
            case 'd': stParms.nNumBase = 10; stParms.fSigned = 1; break;
            case 'i': stParms.nNumBase = 0; stParms.fSigned = 1; break;
            case 'b': /*stParms.nNumBase = 2*/; stParms.fSigned = 0; break; // programable base unsigned
            case 'B': /*stParms.nNumBase = 2*/; stParms.fSigned = 1; break; // programable base   signed
            case 'o': stParms.nNumBase = 8; stParms.fSigned = 0; break;
            case 'u': stParms.nNumBase = 10; stParms.fSigned = 0; break;
            case 'x': stParms.nNumBase = 16; stParms.fSigned = 0; break;
            case 'X': stParms.nNumBase = 16; stParms.fSigned = 0; break;
            case 'p': stParms.nNumBase = 16; stParms.fSigned = 0; dstsize = sizeof(void*); break;
            }

            nTmp = str2num(&stParms, pfnDevGetChar, pfnDevUngetChar, pOverallCount, &pSrc, pFixParm->EndMarker);
            state = (nTmp < 1 ? PROCESS_ERROR : PROCESS_FORMATSTR);  //set new state depending on error

            if (0 == stParms.fAsterix)
                if (nTmp > 0)
                    nTokenScanned++;

            if (state == PROCESS_ERROR)
            {
                if (EARLY_MATCHING_FAILURE == nTmp) // "event of an early matching failure": 
                                                    // http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=299
                    nTokenScanned = 0;
                else
                    nTokenScanned = nTokenScanned == 0 ? -1 : nTokenScanned;
                break;
            }

            if (0 == stParms.fAsterix) {
                pVoid = va_arg(ap, void*);
                switch (dstsize) {
                case DST8: *((char*)pVoid) = (char)stParms.xwNumber; break;
                case DST16: *((short*)pVoid) = (short)stParms.xwNumber; break;
                case DST32: *((int*)pVoid) = (long)stParms.xwNumber; break;
                case DST64: *((long long*)pVoid) = (long long)stParms.xwNumber; break;
                case DSTDFLT:*((int*)pVoid) = (int)stParms.xwNumber; break;
                }
            }
            break;
        case PROCESS_COUNT: state = PROCESS_FORMATSTR;
            if (1 == stParms.fAsterix)
                break;
            pInt = va_arg(ap, int*);
            *pInt = OverallCount;
            break;
        case PROCESS_ERROR:
            nTokenScanned = nTokenScanned == 0 ? 0 : nTokenScanned;
            state = PROCESS_EXIT;
            break;
        case PROCESS_END:
            //nTokenScanned = nTokenScanned == 0 ? EOF : nTokenScanned;
            state = PROCESS_EXIT;
            break;
        }
    }//c = fWide ? pszFormatW[i] : pszFormat[i] != '\0')

    if (NULL != (void*)ppEnd) {
        *ppEnd = -1 == nTokenScanned ? pSrcOriginal : &pSrc[OverallCount];
    }

    return nTokenScanned;
}
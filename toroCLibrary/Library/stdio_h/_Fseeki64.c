/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _Fseeki64.c

Abstract:

    Implementation of the Standard C function.
    Set file position indicator.

Author:

    Kilian Kegel

--*/

#include <cde.h>
#include <CdeServices.h>
//
// stdio.h
//
#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0
#define EOF    (-1)
extern __declspec(dllimport) int fsetpos(FILE* stream, const fpos_t* pos);


extern int __cdeIsFilePointer(void* stream);

/** Synopsis
    #include <stdio.h>
    int _fseeki64(FILE* stream, __int64 offset, int bias);
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fseek-fseeki64?view=msvc-170
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/fseek-lseek-constants?view=msvc-160&viewFallbackFrom=vs-2019
    The fseek function sets the file position indicator for the stream pointed to by stream.
    If a read or write error occurs, the error indicator for the stream is set and fseek fails.
    For a binary stream, the new position, measured in characters from the beginning of the
    file, is obtained by adding offset to the position specified by whence. The specified
    position is the beginning of the file if whence is SEEK_SET, the current value of the file
    position indicator if SEEK_CUR, or end-of-file if SEEK_END. A binary stream need not
    meaningfully support fseek calls with a whence value of SEEK_END.
    For a text stream, either offset shall be zero, or offset shall be a value returned by
    an earlier successful call to the ftell function on a stream associated with the same file
    and whence shall be SEEK_SET.
    After determining the new position, a successful call to the fseek function undoes any
    effects of the ungetc function on the stream, clears the end-of-file indicator for the
    stream, and then establishes the new position. After a successful fseek call, the next
    operation on an update stream may be either input or output.
Returns
    The fseek function returns nonzero only for a request that cannot be satisfied.

**/
int _fseeki64(FILE* stream, __int64 offset, int bias) {

    CDEFILE* pCdeFile = (CDEFILE*)stream;
    int nRet = EOF/*0 == success*/;
    CDEFPOS_T fposnew = { .fpos64 = offset };
    //
    //NOTE: writing to CDEFPOS_T.fpos64 overwrites all bitfields.
    //      AFTERWARDS CDEFPOS_T.Bias mus be initialized and keeps the CDEFPOS_T.Sign
    // 
    //typedef union tagCDEFPOS_T // CDE DEBUG FILE POINTER
    //{
    //    int64_t fpos64;
    //    struct {
    //        uint64_t Offs : 60;
    //        uint64_t Sign : 1;
    //        uint64_t Bias : 3;
    //    }CdeFposBias;
    //}CDEFPOS_T;
    // 
    //enum _CDEFPOSTTYPE {
    //    CDE_SEEK_BIAS_LESS_POS /* 000b */,
    //    CDE_SEEK_BIAS_SET = 4  /* 100b */,
    // 
    //    CDE_SEEK_BIAS_END      /* 110b */,
    //    CDE_SEEK_BIAS_LESS_NEG,/* 111b */

    //};

    do {/*1. dowhile(0)*/

        if (!__cdeIsFilePointer(pCdeFile))
            break;

        nRet = 0;
        switch (bias) {
        case    SEEK_SET:   fposnew.CdeFposBias.Bias = CDE_SEEK_BIAS_SET;
            break;
        case    SEEK_END:   fposnew.CdeFposBias.Bias = CDE_SEEK_BIAS_END;
            break;
        case    SEEK_CUR:   fposnew.fpos64 = pCdeFile->bpos + pCdeFile->bidx + offset;      // NOTE: UEFI SetPosition() dosn't support different origins/bias
                            fposnew.CdeFposBias.Bias = CDE_SEEK_BIAS_SET;                   // https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf#page=600
            break;
        default:
            //TODO: ADD error handler
            nRet = EOF;
            break;
        }

        //
        // NOTE KG20220419: _fseeki64() internally uses fsetpos().
        //
        nRet = fsetpos((FILE*)pCdeFile, (fpos_t*) & fposnew);

        pCdeFile->fEof = 0; //TODO: check iffseek() to SEEK_END + x clear fEof too

    } while (0)/*1. dowhile(0)*/;
    return nRet;
}
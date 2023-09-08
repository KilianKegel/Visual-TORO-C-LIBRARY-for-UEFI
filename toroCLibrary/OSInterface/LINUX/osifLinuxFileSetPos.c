//###################################################################################
//### THIS MODULE IS UNDER CONSTRUCTION AND CURRENTLY EXCLUDED FROM RELEASE BUILD ###
//###################################################################################
/*!
\copyright

    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.

\mainpage osifLinuxFileSetPos

\brief Implementation of the <em>osifLinuxFileSetPos</em> function

\file osifLinuxFileSetPos.c

*/
#include <stdio.h>
#include <stdbool.h>
#include <CdeServices.h>
//
// prototype
//
OSIFFSETPOS _osifLinuxFileSetPos;

extern int64_t  _syscall(int64_t nr, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);
extern fpos_t   __cdeOffsetCdeFposType(fpos_t fpos);
extern int      __cdeBiasCdeFposType(fpos_t fpos);

#define CDE_FPOS_SEEKEND (1LL << 63)

#define LINUX_SEEK_SET 0
#define LINUX_SEEK_CUR 1
#define LINUX_SEEK_END 2

int _osifLinuxFileSetPos(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, CDEFPOS_T* pos) {

    bool f = 1;
    fpos_t eofpos = 0LL, newpos = __cdeOffsetCdeFposType(pos->fpos64);
    uint32_t dwMoveMethod = __cdeBiasCdeFposType(pos->fpos64);

    do {

        if (pCdeFile->openmode & O_CDENOSEEK    /* if e.g. the file is a console, don't try to seek that will fail */)
            break;

        if ((CDE_SEEK_BIAS_APPEND & CDE_SEEK_BIAS_MSK) == dwMoveMethod)
            newpos = 0LL,
            dwMoveMethod = LINUX_SEEK_END;

        newpos = _syscall(/* sys_lseek */ 8, (int64_t)pCdeFile->emufp, newpos, dwMoveMethod, 0, 0, 0);

        if (-1LL == newpos)
            f = 0;

    } while (0);

    pCdeFile->bpos = newpos;

    return f ? 0 : EOF;
}
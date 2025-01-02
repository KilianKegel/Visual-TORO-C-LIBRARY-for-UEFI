/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    wcsftime.c

Abstract:

    Implementation of the Standard C function.
    Format a time string.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <time.h>

extern __declspec(dllimport) int sprintf(char* pszBuffer, const char* pszFormat, ...);
extern __declspec(dllimport) size_t strlen(const char* pszBuffer);
extern __declspec(dllimport) size_t mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count);
extern void* __cdeGetAppIf(void);

extern char* wday_name_short[7];
extern char* mon_name_short[12];
extern char* wday_name_long[7];
extern char* mon_name_long[12];
extern char* ampm[2];

static int FirstYday(int year_since_1900) {
    struct tm tm = { 0,0,0,1,0,0,0,0,0 };

    tm.tm_year = year_since_1900;
    mktime(&tm);
    return tm.tm_wday;

}

static int Sun2MonBased(int wday) {
    return (wday + 6) % 7;
}

/** wcsftime() -- string format time

Synopsis
    #include <time.h>
    size_t wcsftime(wchar_t *strDest, size_t maxsize, const wchar_t *format, const struct tm *timeptr);

Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/wcsftime-wcsftime-wcsftime-l-wcsftime-l
    https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=397
    The wcsftime function places characters into the array pointed to by s
    as controlled by the string pointed to by format.

Returns
    If the total number of resulting characters including the terminating null character is not
    more  than maxsize, the wcsftime function  returns  the  number  of  characters  placed
    into  the  array  pointed  to  by s not  including  the  terminating  null  character.
    Otherwise, zero is returned and the contents of the array are indeterminate.

    @param[in] char *s
    @param[in] size_t maxsize
    @param[in] const char * format
    @param[in] const struct tm * ptm

    @retval size_t, number of bytes written, excluding termination zero

**/
static size_t wcsftimeCDEABI(wchar_t* pDest, size_t maxsize, const wchar_t* format, const struct tm* ptm)
{
    size_t nRet = 0;
    int n, f, fSharp, fErr, fFinished, l;
    char buffer[64];
    wchar_t c;
    enum STATE {
        PROCESS_WRITECHARS,
        PROCESS_PERCENT,
        PROCESS_FORMAT,
        PROCESS_ERROR,
        PROCESS_COPYBUFFER,
        PROCESS_SHARP
    }state = PROCESS_WRITECHARS;

    do {

        // ----- check parameters

        if (NULL == pDest ||
            (int)ptm->tm_year < 70 ||
            (int)ptm->tm_sec  > 60 ||
            (int)ptm->tm_min > 60  ||
            (int)ptm->tm_hour > 23 ||
            (int)ptm->tm_mday > 31 ||
            (int)ptm->tm_mday < 1  ||
            (int)ptm->tm_mon  > 12 ||
            (int)ptm->tm_wday > 6  ||
            (int)ptm->tm_yday > 366) {
            nRet = 0;
            if (pDest)
                pDest[0] = '\0';
            break;
        }
        for (n = f = fErr = fFinished = 0, state = PROCESS_WRITECHARS; n < (int)maxsize && fErr == 0 && fFinished == 0; ) {
            c = format[f];
            switch (state) {
            case PROCESS_WRITECHARS:
                fSharp = 0;
                if (c == '%') {
                    state = PROCESS_PERCENT;
                }
                else if (c == '#') {
                    state = PROCESS_SHARP;
                }
                else {
                    pDest[n++] = c;
                    fFinished = (c == '\0');
                }
                f++;
                break;
            case PROCESS_PERCENT:

                if (c == '\0') {
                    state = PROCESS_ERROR;
                    break;
                }

                if (c == '%') {
                    pDest[n++] = c;
                    state = PROCESS_WRITECHARS;
                    f++;
                    break;
                }

                state = PROCESS_FORMAT;
                break;
            case PROCESS_SHARP:

                fSharp = 1;
                state = PROCESS_FORMAT;
                break;

            case PROCESS_ERROR:
                if (0 != maxsize)
                    pDest[0] = '\0';
                n = 0;
                fErr = 1;
                break;

            case PROCESS_FORMAT:
                switch (c) {

                case 'a':   f++; sprintf(buffer, "%s", wday_name_short[ptm->tm_wday]); break;
                case 'A':   f++; sprintf(buffer, "%s", wday_name_long[ptm->tm_wday]); break;
                case 'b':   f++; sprintf(buffer, "%s", mon_name_short[ptm->tm_mon]); break;
                case 'B':   f++; sprintf(buffer, "%s", mon_name_long[ptm->tm_mon]); break;
                case 'c':   f++; sprintf(buffer, "%s %s %2d %02d:%02d:%02d %04d", wday_name_short[ptm->tm_wday], mon_name_short[ptm->tm_mon], ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, 1900 + ptm->tm_year); break;
                case 'd':   f++; sprintf(buffer, "%02d", ptm->tm_mday); break;
                case 'H':   f++; sprintf(buffer, "%02d", ptm->tm_hour); break;
                case 'I':   f++; sprintf(buffer, "%02d", ptm->tm_hour % 12 == 0 ? 12 : ptm->tm_hour % 12); break;
                case 'j':   f++; sprintf(buffer, "%03d", 1 + ptm->tm_yday); break;
                case 'm':   f++; sprintf(buffer, "%02d", ptm->tm_mon + 1); break;
                case 'M':   f++; sprintf(buffer, "%02d", ptm->tm_min); break;
                case 'p':   f++; sprintf(buffer, "%s", ampm[ptm->tm_hour >= 12]); break;
                case 'S':   f++; sprintf(buffer, "%02d", ptm->tm_sec); break;
                case 'U':   f++; sprintf(buffer, "%02d", (0 == FirstYday(ptm->tm_year)) + ((FirstYday(ptm->tm_year) - ptm->tm_wday) > 0) + ptm->tm_yday / 7); break;
                case 'w':   f++; sprintf(buffer, "%d", ptm->tm_wday); break;
                case 'W':   f++; sprintf(buffer, "%02d", (1 == FirstYday(ptm->tm_year)) + (Sun2MonBased(FirstYday(ptm->tm_year)) - Sun2MonBased(ptm->tm_wday) > 0) + ptm->tm_yday / 7); break;
                case 'x':   f++; sprintf(buffer, "%02d/%02d/%02d", 1 + ptm->tm_mon, ptm->tm_mday, ptm->tm_year % 100); break;
                case 'X':   f++; sprintf(buffer, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec); break;
                case 'y':   f++; sprintf(buffer, "%02d", ptm->tm_year % 100); break;
                case 'Y':   f++; sprintf(buffer, "%d", 1900 + ptm->tm_year); break;
                case 'Z':   f++; sprintf(buffer, "Coordinated Universal Time"); break;

                default: state = PROCESS_ERROR; break;
                }
                state = PROCESS_COPYBUFFER;
                break;
            case PROCESS_COPYBUFFER:

                l = (int)strlen(buffer);

                if (l + n < (int)maxsize) {
                    mbstowcs(&pDest[n], buffer, (size_t)l);
                    n += (int)l;
                    state = PROCESS_WRITECHARS;
                }
                else {
                    state = PROCESS_ERROR;
                }
                break;
            }
        }

        if (n >= (int)maxsize && '\0' != pDest[n - 1])
            n = 0;
        if (n)
            n--;
        if (maxsize)
            pDest[n] = '\0';

        nRet = n;

    } while (0);

    return nRet;
}

MKCDEABI(wcsftime);
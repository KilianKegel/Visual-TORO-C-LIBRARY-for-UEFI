/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    wctype.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=405

Author:

    Kilian Kegel

--*/
#ifndef _CDE_WCTYPE_H_
#define _CDE_WCTYPE_H_
#include <stddef.h>

typedef unsigned short wint_t;
typedef unsigned short wctype_t;
typedef unsigned short wctrans_t;

#define WEOF ((wint_t)(0xFFFF))


int iswalnum( wint_t wc);
int iswalpha( wint_t wc);
int iswblank( wint_t wc );
int iswctype( wint_t wc, wctype_t desc );
int iswcntrl( wint_t wc);
int iswdigit( wint_t wc);
int iswgraph( wint_t wc);
int iswlower( wint_t wc);
int iswprint( wint_t wc);
int iswpunct( wint_t wc);
int iswspace( wint_t wc);
int iswupper( wint_t wc);
int iswxdigit( wint_t wc );

wctype_t wctype( const char *property );
wctrans_t wctrans( const char *property );

wint_t  towlower( wint_t wc );
wint_t  towupper( wint_t wc );

wint_t towctrans(wint_t c, wctrans_t value);

// Microsoft specific - start
// Bit masks for the possible character types
#define _UPPER   0x01     // uppercase letter
#define _LOWER   0x02     // lowercase letter
#define _DIGIT   0x04     // digit[0-9]
#define _SPACE   0x08     // tab, carriage return, newline, vertical tab, or form feed
#define _PUNCT   0x10     // punctuation character
#define _CONTROL 0x20     // control character
#define _BLANK   0x40     // space char (tab is handled separately)
#define _HEX     0x80     // hexadecimal digit

#define _LEADBYTE 0x8000                    // multibyte leadbyte
#define _ALPHA   (0x0100 | _UPPER | _LOWER) // alphabetic character

// Microsoft specific - end

#endif//_CDE_WCTYPE_H_

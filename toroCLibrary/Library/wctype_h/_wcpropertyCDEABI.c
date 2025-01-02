/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeWcproperty.c

Abstract:

    Toro C Library wide character function property cross bar

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
//
//#include <wctype.h>
//
#define _UPPER   0x01
#define _LOWER   0x02
#define _DIGIT   0x04
#define _SPACE   0x08
#define _PUNCT   0x10
#define _CONTROL 0x20
#define _BLANK   0x40
#define _HEX     0x80
#define _ALPHA   (0x0100 | _UPPER | _LOWER)

#include <_wctypeCDEABI.h>

#if   defined(_M_AMD64)

    extern int (*__imp_iswprint)(wint_t);
    extern int (*__imp_iswgraph)(wint_t);
    extern int (*__imp_iswalnum)(wint_t);
    extern int (*__imp_iswalpha)(wint_t);
    extern int (*__imp_iswupper)(wint_t);
    extern int (*__imp_iswlower)(wint_t);
    extern int (*__imp_iswdigit)(wint_t);
    extern int (*__imp_iswspace)(wint_t);
    extern int (*__imp_iswpunct)(wint_t);
    extern int (*__imp_iswcntrl)(wint_t);
    extern int (*__imp_iswblank)(wint_t);
    extern int (*__imp_iswxdigit)(wint_t);

    WCPROPERTYCDEABI __cdeWcpropertyCDEABI[] = { \
                    {/*0x0157*/_ALPHA |        _BLANK |            _PUNCT |          _DIGIT | _LOWER | _UPPER, &__imp_iswprint ,"print" },\
                    {/*0x0117*/_ALPHA |                            _PUNCT |          _DIGIT | _LOWER | _UPPER, &__imp_iswgraph ,"graph" },\
                    {/*0x0107*/_ALPHA |                                              _DIGIT | _LOWER | _UPPER, &__imp_iswalnum ,"alnum" },\
                    {/*0x0103*/_ALPHA |                                                       _LOWER | _UPPER, &__imp_iswalpha ,"alpha" },\
                    {/*0x0001*/                                                                        _UPPER, &__imp_iswupper ,"upper" },\
                    {/*0x0002*/                                                               _LOWER         , &__imp_iswlower ,"lower" },\
                    {/*0x0004*/                                                      _DIGIT                  , &__imp_iswdigit ,"digit" },\
                    {/*0x0008*/                                             _SPACE                           , &__imp_iswspace ,"space" },\
                    {/*0x0010*/                                    _PUNCT                                    , &__imp_iswpunct ,"punct" },\
                    {/*0x0020*/                         _CONTROL                                             , &__imp_iswcntrl ,"cntrl" },\
                    {/*0x0040*/                _BLANK                                                        , &__imp_iswblank ,"blank" },\
                    {/*0x0080*/         _HEX                                                                 , &__imp_iswxdigit,"xdigit"},\
                    {/*0x0000*/                                                                             0, NULL    ,NULL    }\
    };
#else//   defined(_M_AMD64)

    extern int (*_imp__iswprint)(wint_t);
    extern int (*_imp__iswgraph)(wint_t);
    extern int (*_imp__iswalnum)(wint_t);
    extern int (*_imp__iswalpha)(wint_t);
    extern int (*_imp__iswupper)(wint_t);
    extern int (*_imp__iswlower)(wint_t);
    extern int (*_imp__iswdigit)(wint_t);
    extern int (*_imp__iswspace)(wint_t);
    extern int (*_imp__iswpunct)(wint_t);
    extern int (*_imp__iswcntrl)(wint_t);
    extern int (*_imp__iswblank)(wint_t);
    extern int (*_imp__iswxdigit)(wint_t);

    WCPROPERTYCDEABI __cdeWcpropertyCDEABI[] = { \
                    {/*0x0157*/_ALPHA |        _BLANK |            _PUNCT |          _DIGIT | _LOWER | _UPPER, &_imp__iswprint ,"print" },\
                    {/*0x0117*/_ALPHA |                            _PUNCT |          _DIGIT | _LOWER | _UPPER, &_imp__iswgraph ,"graph" },\
                    {/*0x0107*/_ALPHA |                                              _DIGIT | _LOWER | _UPPER, &_imp__iswalnum ,"alnum" },\
                    {/*0x0103*/_ALPHA |                                                       _LOWER | _UPPER, &_imp__iswalpha ,"alpha" },\
                    {/*0x0001*/                                                                        _UPPER, &_imp__iswupper ,"upper" },\
                    {/*0x0002*/                                                               _LOWER         , &_imp__iswlower ,"lower" },\
                    {/*0x0004*/                                                      _DIGIT                  , &_imp__iswdigit ,"digit" },\
                    {/*0x0008*/                                             _SPACE                           , &_imp__iswspace ,"space" },\
                    {/*0x0010*/                                    _PUNCT                                    , &_imp__iswpunct ,"punct" },\
                    {/*0x0020*/                         _CONTROL                                             , &_imp__iswcntrl ,"cntrl" },\
                    {/*0x0040*/                _BLANK                                                        , &_imp__iswblank ,"blank" },\
                    {/*0x0080*/         _HEX                                                                 , &_imp__iswxdigit,"xdigit"},\
                    {/*0x0000*/                                                                             0, NULL    ,NULL    }\
    };
#endif//  defined(_M_AMD64)

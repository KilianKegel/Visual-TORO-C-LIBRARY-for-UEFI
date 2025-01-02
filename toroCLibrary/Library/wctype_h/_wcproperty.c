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

#include <wctype.h>
#include <_wctype.h>

WCPROPERTY __cdeWcproperty[] = { \
                    {/*0x0157*/_ALPHA |        _BLANK |            _PUNCT |          _DIGIT | _LOWER | _UPPER, iswprint ,"print" },\
                    {/*0x0117*/_ALPHA |                            _PUNCT |          _DIGIT | _LOWER | _UPPER, iswgraph ,"graph" },\
                    {/*0x0107*/_ALPHA |                                              _DIGIT | _LOWER | _UPPER, iswalnum ,"alnum" },\
                    {/*0x0103*/_ALPHA |                                                       _LOWER | _UPPER, iswalpha ,"alpha" },\
                    {/*0x0001*/                                                                        _UPPER, iswupper ,"upper" },\
                    {/*0x0002*/                                                               _LOWER         , iswlower ,"lower" },\
                    {/*0x0004*/                                                      _DIGIT                  , iswdigit ,"digit" },\
                    {/*0x0008*/                                             _SPACE                           , iswspace ,"space" },\
                    {/*0x0010*/                                    _PUNCT                                    , iswpunct ,"punct" },\
                    {/*0x0020*/                         _CONTROL                                             , iswcntrl ,"cntrl" },\
                    {/*0x0040*/                _BLANK                                                        , iswblank ,"blank" },\
                    {/*0x0080*/         _HEX                                                                 , iswxdigit,"xdigit"},\
                    {/*0x0000*/                                                                             0, NULL    ,NULL    }\
};
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    _cdeStdCIntrinsics_c.h

Abstract:
    
    Standard C functions, internally used by the C compiler (so called "intrinsics")
    in their __cdecl and __declspec(dllimport) incarnation both, bound to the same COMDAT,
    provided here.

    NOTE: In "CdeLib.lib" all Standard C Functions are provided in their __decspec(dllimport)
          incarnation only, except Standard C Functions internally used as intrinsics
          by the Microsoft C Compiler, e.g. memset(), memcmp()!
    
    -----------------------------------------------------------------------------------------
          This is to avoid _any_ conflict with UEFI tianocores pseudo C library surrogate
    -----------------------------------------------------------------------------------------

Author:

    Kilian Kegel

--*/

#ifndef _CDE_STDC_INTRINSICS_C_H_
#define _CDE_STDC_INTRINSICS_C_H_

//
// NOTE: the xyzCDEINTRINABI.c files contains both, the __cdecl AND the __declspec(dllimport)
//       version in one, single COMDAT
//
extern char __cdeMemSetCDEINTRINABIAnchor;
extern char __cdeMemCmpCDEINTRINABIAnchor;
extern char __cdeLONGJMPCDEINTRINABIAnchor;
extern char __cdeSETJMPCDEINTRINABIAnchor;


char * rgPullInCDEINTRINABI[] = {
    &__cdeMemSetCDEINTRINABIAnchor,
    &__cdeMemCmpCDEINTRINABIAnchor,
    &__cdeLONGJMPCDEINTRINABIAnchor,
    &__cdeSETJMPCDEINTRINABIAnchor
};

#endif//_CDE_STDC_INTRINSICS_C_H_

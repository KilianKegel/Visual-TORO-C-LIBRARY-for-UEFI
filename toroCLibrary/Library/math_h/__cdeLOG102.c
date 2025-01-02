/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __cdeLOG2E.c

Abstract:

    CDE internal math constant decimal logarithm of 2.0

Author:

    Kilian Kegel

--*/            
#include <corecrt_math_defines.h>
const double __cdeLOG102 = 0.30102999566398119521373889472449;  // M_LOG102: MSFT did not define this constant in math.h
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    __isa_available.c

Abstract:

    Microsoft specific intrinsic function

Author:

    Kilian Kegel

--*/
#include <isa_availability.h>
unsigned int __isa_available = __ISA_AVAILABLE_SSE42;
/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    new.c

Abstract:

    Implementation of the C++ delete operator

Author:

    Kilian Kegel

--*/
#include <stdlib.h>

void operator delete(void* ptr, size_t size)
{
    free(ptr);
}

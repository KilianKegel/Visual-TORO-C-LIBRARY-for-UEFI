/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Labs.c

Abstract:

    Implementation of the Standard C function.
    Calculates the absolute value of the argument.

Author:

    Kilian Kegel

--*/

/*++

Synopsis
    #include <stdlib.h>
    long labs( long n );
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/abs-labs-llabs-abs64?view=msvc-160&viewFallbackFrom=vs-2019
Parameters
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/abs-labs-llabs-abs64?view=msvc-160#parameters
Returns
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/abs-labs-llabs-abs64?view=msvc-160#return-value

*/
long labs(long i) {
    return i < 0 ? -i : i;
}
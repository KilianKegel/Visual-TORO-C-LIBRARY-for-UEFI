/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    QsortCDEABI.c

Abstract:

    Import Library version

    Implementation of the Standard C function.
    Performs a sort.

Author:

    Kilian Kegel

--*/
#include <CdeServices.h>
#include <stddef.h>

#define ELC(x) (sizeof(x) / sizeof(x[0]))

/**

Synopsis
    #include <stdlib.h>
    void qsort(void *base, size_t nmemb, size_t size,int (*compar)(const void *, const void *));
Description
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-160&viewFallbackFrom=vs-2019
    The qsort function  sorts  an  array  of nmemb objects,  the  initial  element  of  which  is
    pointed to by base. The size of each object is specified by size.

    The  contents  of  the  array  are  sorted  into  ascending  order  according  to  a  comparison
    function  pointed  to  by compar,  which  is  called  with  two  arguments  that  point  to  the
    objects  being  compared.   The  function  shall  return  an  integer  less  than,  equal  to,  or
    greater than zero if the first argument is considered to be respectively less than, equal to,
    or greater than the second.

    If two elements compare as equal, their order in the resulting sorted array is unspecified.
Returns
    The qsort function returns no value.

**/
static int swap(const void* p01, const void* p02, size_t width)
{
    char* p1 = (void*)p01, * p2 = (void*)p02;
    char c;
    int i;

    for (i = 0; i < (int)width; i++)
        c = p1[i], p1[i] = p2[i], p2[i] = c;
    return 0;
}

static void qsortCDEABI(void* base, size_t num, size_t width, int (*compar) (const void*, const void*)) {

    char* p1, * p2, * px1, * px2, * pb;
    int diff;
    size_t n;

    while (num--) {

        for (pb = p1 = (char*)base, p2 = &p1[width], n = 0, px1 = px2 = NULL; p2 < &pb[width * (num + 1)]; p2 = &p2[width], n++/*, px1 = NULL*/)
        {

            diff = (*compar)(p2, p1);

            if (diff > 0)
                p1 = p2;

            if (diff < 0)
                px1 = p1, px2 = p2;
        }

        if (NULL != px1)
            swap(px1, px2, width);

    };
}

MKCDEABI(qsort);
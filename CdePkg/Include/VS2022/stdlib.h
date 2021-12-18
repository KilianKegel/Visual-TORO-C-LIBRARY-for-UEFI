/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    stdlib.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=318

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STDLIB_H_
#define _CDE_STDLIB_H_
#include <stddef.h>

typedef struct _div_t
{
	int quot;
	int rem;
} div_t;

typedef struct _ldiv_t
{
	long quot;
	long rem;
} ldiv_t;

typedef struct _lldiv_t
{
	long long quot;
	long long rem;
} lldiv_t;	/* C99 */

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 0x7fff
#define MB_CUR_MAX 1 /* C locale only */

double atof( const char *nptr );
int atoi( const char *nptr );
long int atol( const char *nptr );
double strtod( const char * nptr, char ** endptr );
long int strtol( const char * nptr, char ** endptr, int base );
unsigned long int strtoul( const char * nptr, char ** endptr, int base );
int rand( void );
void srand( unsigned int seed );
void *calloc( size_t nmemb, size_t size );
void free( void *ptr );
void *malloc( size_t size );
void *realloc( void *ptr, size_t size );
void abort( void );
int atexit( void ( *func )( void ) );
void exit( int status );
char *getenv( const char *name );
int system( const char *string );
void *bsearch( const void *key, const void *base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) );
void qsort( void *base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) );
int abs( int j );
long int labs( long int j );
div_t div( int numer, int denom );
ldiv_t ldiv( long int numer, long int denom );
int mblen( const char *s, size_t n );
int mbtowc( wchar_t * pwc, const char * s, size_t n );
int wctomb( char *s, wchar_t wchar );
size_t mbstowcs( wchar_t * pwcs, const char * s, size_t n );
size_t wcstombs( char * s, const wchar_t * pwcs, size_t n );

#endif//_CDE_STDLIB_H_

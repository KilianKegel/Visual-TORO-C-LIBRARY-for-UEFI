/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2022, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    string.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=337

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STRING_H_
#define _CDE_STRING_H_

#include <stddef.h>

void *memcpy( void * s1, const void * s2, size_t n );
void *memmove( void *s1, const void *s2, size_t n );
char *strcpy( char * s1, const char * s2 );
char *strncpy( char * s1, const char * s2, size_t n );
char *strcat( char * s1, const char * s2 );
char *strncat( char * s1, const char * s2, size_t n );
int memcmp( const void *s1, const void *s2, size_t n );
int strcmp( const char *s1, const char *s2 );
int strcoll( const char *s1, const char *s2 );
int strncmp( const char *s1, const char *s2, size_t n );
size_t strxfrm( char * s1, const char * s2, size_t n );
void *memchr( const void *s, int c, size_t n );
char *strchr( const char *s, int c );
size_t strcspn( const char *s1, const char *s2 );
char *strpbrk( const char *s1, const char *s2 );
char *strrchr( const char *s, int c );
size_t strspn( const char *s1, const char *s2 );
char *strstr( const char *s1, const char *s2 );
char *strtok( char * s1, const char * s2 );
void *memset( void *s, int c, size_t n );
char *strerror( int errnum );
size_t strlen( const char *s );

#endif//_CDE_STRING_H_

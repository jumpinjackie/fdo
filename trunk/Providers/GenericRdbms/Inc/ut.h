// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
*                                                                       *
* Name                                                                  *
*   ut.h - header defining common macros and functions                  *
*                                                                       *
* Synopsis                                                              *
*   #include    <Inc/ut.h>                                              *
*                                                                       *
* Description                                                           *
*       This  header  defines  the macros and functions which are       *
*       required by many applications,  and  are  not application       *
*       specific.                                                       *
*                                                                       *
* Parameters                                                            *
*   N/A                                                                 *
*                                                                       *
* Function value                                                        *
*   N/A                                                                 *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*                                                                       *
************************************************************************/

#ifndef UT_H
#define UT_H    1

#include    <Inc/tfn.h>

typedef void * types_addr_t;

#ifndef FILE
#include    <stdio.h>
#endif
#include <string.h>
#include <malloc.h>

#ifndef HUGE_VAL
#include    <math.h>                        /* needed for atan2, M_PI */
#endif  /* HUGE_VAL */

        /***    constants   ***/

#define UT_LINE_SIZE        1024

#define WHITE_SPACE         " \t\n\f"
#define SINGLE_QUOTE        '\047'
#define DOUBLE_QUOTE        '\042'
#define HUGEINT             0x7fffffff      /* largest positive int */
#ifndef MS_WINDOWS
#define TINYINT             0x80000000      /* largest negative int */
#else
#define TINYINT             INT_MIN
#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#define ut_tolower(c)           (tolower((unsigned char) (c)))
#define ut_toupper(c)           (toupper((unsigned char) (c)))
#define ut_toascii(c)           (toascii((unsigned char) (c)))
#define ut_isalnum(c)           (isalnum((unsigned char) (c)))
#define ut_isalpha(c)           (isalpha((unsigned char) (c)))
#define ut_iscntrl(c)           (iscntrl((unsigned char) (c)))
#define ut_isprint(c)           (isprint((unsigned char) (c)))
#define ut_isspace(c)           (isspace((unsigned char) (c)))
#define ut_isdigit(c)           (isdigit((unsigned char) (c)))
#define ut_isxdigit(c)          (isxdigit((unsigned char) (c)))
#define ut_isupper(c)           (isupper((unsigned char) (c)))
#define ut_islower(c)           (islower((unsigned char) (c)))
#define ut_isascii(c)           (isascii((unsigned char) (c)))
#define ut_ispunct(c)           (ispunct((unsigned char) (c)))

#define ut_vm_malloc( a, b )	malloc( b )
#define ut_vm_free( a, b )		free( b )

#define ut_to_upper( str ) { \
		char* ptr; \
		for (ptr = str; *ptr != '\0'; ptr++) \
			*ptr = ut_toupper(*ptr);  \
	}

#define  ut_to_lower( string ) { \
    if (string != NULL) {	\
        for (; *string != '\0'; string++)   \
            *string = ut_tolower(*string);	\
	} }

/* function prototypes */
char * ut_itoa( int i,  char * dst );

char * ut_ltoa( long l, char * dst );

char * ut_dtoa_p( double d, int p, char   *ptr );

char * ut_dtoa( double n, char   *ptr  );

#endif


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

#ifndef INC_UT_DA_H
#define INC_UT_DA_H

/************************************************************************
* Name                                                                  *
*   ut/da.h - Dynamic Array package definitions                         *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/ut/da.h>                                              *
*                                                                       *
* Description                                                           *
*       This   file   contains   structure  definitions  used  in       *
*       the ut_da_*() suite of functions, and by their callers.         *
*                                                                       *
* Remarks                                                               *
*       None.                                                           *
*                                                                       *
************************************************************************/

#include <stdlib.h>

#if (!defined __cplusplus && !defined c_plusplus)
    typedef int         bool;

#define		true	1
#define		false	0
#endif

typedef struct {                    /* Dynamic Array        */
    long             el_size;       /* Element size (bytes) */
    void            *data;          /* Dynamic array data   */
    long             size;          /* # elements used      */
    long             allocated;     /* # elements allocated */
} ut_da_def;


void *ut_da_append( ut_da_def *da, long num_el, void *data );

void *ut_da_insert( ut_da_def *da, long indx,   void *data );

void *ut_da_get(    ut_da_def *da, long indx );

void *ut_da_get2D(  ut_da_def *da, long x, long y );

bool  ut_da_delete( ut_da_def *da, long indx, long num_el );

bool  ut_da_free(   ut_da_def *da );

bool  ut_da_free2D( ut_da_def *da );

bool  ut_da_check(  ut_da_def *da );

bool  ut_da_presize( ut_da_def *da, long num_el );

bool  ut_da_presize2D( ut_da_def *da, long el_size, long num_rows,
                                                           long num_columns );

#define      UT_DA_INDX(da, ptr)    /* Index of element at ptr */   \
                        (((char *)(ptr) - (char *)(da)->data) / (da)->el_size)

#define      UT_DA_GET_FAST(da, i)  /* Fast, unchecked retrieval */ \
                        ((char *)(da)->data + ((i)*(da)->el_size))

#define      UT_DA_GET2D_FAST(da, x, y) /* Fast, unchecked retrieval */ \
                        ((char *) ((((ut_da_def *)(da)->data)+(y)) -> data) + \
                         (x)*((((ut_da_def *)(da)->data)+(y))->el_size))

#endif


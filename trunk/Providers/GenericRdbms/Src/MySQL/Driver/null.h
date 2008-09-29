/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef MYSQL_NULL_H
#define MYSQL_NULL_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

int mysql_alcnullind(
	mysql_context_def *context,
    int                 n,           /* The number of elements to allocate   */
    char                **null_ind   /* address of pointer to allocate       */
    );
void mysql_set_null(
	mysql_context_def  *context,
    my_bool             *null_ind,     /* NULL indicator array                     */
    int                 start,        /* The start index to begin setting NULL    */
    int                 end           /* The end index to set NULL                */
    );
void mysql_set_nnull(
	mysql_context_def  *context,
    my_bool             *null_ind,    /* NULL indicator array                     */
    int                 start,        /* The start index to begin setting NULL    */
    int                 end           /* The end index to set NULL                */
    );
int mysql_is_null(
    mysql_context_def  *context,
	char                *null_ind,
    int                 offset
    );

#endif // GEN_GET_ID_H

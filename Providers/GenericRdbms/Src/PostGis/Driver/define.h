/*
 * Copyright (C) 2006 Refractions Research, Inc. 
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

#ifndef POSTGIS_DEFINE_H
#define POSTGIS_DEFINE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

/**
 * Define destination for data of column from query results.
 *
 * @param context [in] - pointer to current PostGIS session context.
 * @return Returns an rdbi status code from rdbi.h. 
 */
int postgis_define (
    postgis_context_def *context,
    char *cursor,
    char *name,
    int datatype,
    int size,
    char *address,
    short *null_ind);

#endif /* POSTGIS_DEFINE_H */


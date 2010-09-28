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

#ifndef POSTGIS_DESC_SLCT_H
#define POSTGIS_DESC_SLCT_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

/**
 * Describe an element of the select clause.
 * This function uses PQdescribePrepared from libpq API which is supported
 * only in protocol 3.0 and later. The protocol 2.0 is not supported.
 *
 * @param context [in] - pointer to current PostGIS session context.
 * @param position [in] - identifier position within SELECT clause.
 *                        In PostgreSQL, placeholder positions start with 0.
 * @param name_size [in] - maximum size of name.
 * @param name [out] - name of bound variable, identifiers column or expression.
 * @param rdbi_type [out] - RDBI constant representing data type.
 * @param binary_size [out] - bytes of memory to hold it.
 * @param null_ok [out] - TRUE iff NULLs ok in this column.
 * @return RDBI status code.  Information on elements of a select clause.
 */
int postgis_desc_slct (
    postgis_context_def *context,
    char *cursor,
    int   position,
    int   name_size,
    char *name,
    int  *rdbi_type,
    int  *binary_size,
    int  *null_ok);

#endif /* POSTGIS_DESC_SLCT_H */

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

#ifndef POSTGIS_NULL_H
#define POSTGIS_NULL_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

/**
 * Allocate a null indicator array.
 *
 * @param context [in] - pointer to context of current PostGIS session
 * @param n [in] - the number of elements to allocate.
 * @param null_ind [out] - address of pointer to allocate.
 * @return Error code of operation state.
 */
int postgis_alloc_null_ind(
        postgis_context_def *context,
        int n,
        char** null_ind);


/**
 * Set a member of a null indicator array NULL.
 *
 * @param context [in] - pointer to context of current PostGIS session
 * @param null_ind [in] - NULL indicator array.
 * @param start [in] - the start index to begin setting NULL.
 * @param end [in] - the end index to set NULL.
 */
void postgis_set_null(
        postgis_context_def* context,
        char* null_ind,
        int start,
        int end);


/**
 * Set a member of a null indicator array NOT NULL.
 *
 * @param context [in] - pointer to context of current PostGIS session
 * @param null_ind [in] - NULL indicator array.
 * @param start [in] - the start index to begin setting NULL.
 * @param end [in] - the end index to set NULL.

 */
void postgis_set_not_null(
        postgis_context_def* context,
        char* null_ind,
        int start,
        int end);


/**
 * Check the NULL status of a null indicator array member.
 *
 * @param context [in] - pointer to context of current PostGIS session
 * @param null_ind [in] - NULL indicator array.
 * @param offset [in] - When dealing with an individual indicator
 * variable, the index.
 * @return Error code of operation state.
 */
int postgis_is_null(
        postgis_context_def  *context,
        char *null_ind,
        int offset);

#endif /* POSTGIS_NULL_H */

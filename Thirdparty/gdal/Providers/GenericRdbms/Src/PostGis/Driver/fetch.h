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

#ifndef POSTGIS_FETCH_H
#define POSTGIS_FETCH_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"


/**
 * etch row(s) from the rdbms.
 *
 * @param context [in] - pointer to current PostGIS session context.
 * @param cursor [in] - pointer to PostGIS-specific cursor area.
 * @param count [in] - for array processing, indicates how many rows to attempt to fetch.
 *  Currently always 1.
 * @param do_exec [in] - Offset within array to begin at. Currently NOT USED.
 * @param do_cancel [in] - Offset within array to begin at. Currently NOT USED.
 * @param rows_processed [out] - location where to save number of rows actually returned.
 * @return Rdbi error code on failure, RDBI_SUCCESS otherwise.
 */
int postgis_fetch (
    postgis_context_def *context,
    char *cursor,
    int count,
    int do_exec,
    int do_cancel,
    int *rows_processed);

#endif /* POSTGIS_FETCH_H */

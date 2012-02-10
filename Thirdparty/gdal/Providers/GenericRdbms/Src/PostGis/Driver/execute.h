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

#ifndef POSTGIS_EXECUTE_H
#define POSTGIS_EXECUTE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

/**
 * Execute a previously prepared SQL statement.
 *
 * @param context [in] - pointer to current PostGIS session context.
 * @param cursor [in] - pointer to PostGIS-specific cursor area.
 * @param count [in] - number of rows to process. Currently always 1.
 * @param offset [in] - Offset within array to begin at. Currently NOT USED.
 * @param rows_processed [out] - location where number of processed rows
 *  will be written.
 * @return Rdbi error code on failure, RDBI_SUCCESS otherwise.
 */
int postgis_execute (
    postgis_context_def *context,
    char *cursor,
    int count,
    int offset,
    int *rows_processed);

#endif /* POSTGIS_EXECUTE_H */

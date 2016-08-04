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

#ifndef POSTGIS_EST_CURSOR_H
#define POSTGIS_EST_CURSOR_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

/**
 * Allocate and initialize a cursor context area.
 *
 * @param contex [in] - pointer to PostGIS context being deallocated.
 * @param cursor [out] - address of a place to put the address of
 * the allocated and initialized cursor context area.
 * @return Error code of operation state, RDBI_SUCCESS on success.
 */
int postgis_est_cursor (postgis_context_def *context, char **cursor);

/**
 *
 * @param contex - pointer to PostGIS context being deallocated.
 * @param cursor [out] - 
 * @return Error code of operation state, RDBI_SUCCESS on success.
 */
int postgis_est_cursor_obj (postgis_context_def *context, char **cursor);

#endif /* POSTGIS_EST_CURSOR_H */

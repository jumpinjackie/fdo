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
#ifndef POSTGIS_PGRESULT_ERROR_H
#define POSTGIS_PGRESULT_ERROR_H

#include "local.h"

/**
 * Sets PGresult status error message to as current context last error.
 * @param context [out] - pointer to current PostGIS session context.
 *  The function updates passed context by setting last error message.
 * @param pgresult [in] - pointer to last PostgreSQL command result.
 */
void postgis_pgresult_error (
    postgis_context_def *context,
    const PGresult* pgresult
);

#endif /* POSTGIS_PGRESULT_ERROR_H */
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

#ifndef POSTGIS_SET_DATABASE_H
#define POSTGIS_SET_DATABASE_H

#include "local.h"

/**
 * Change database for current session.
 *
 * @param context [in] - pointer to PostGIS session context.
 * @param database_name [in] - name of PostgreSQL database being switched to
 *  for the current session.
 * @return RDBI_SUCCESS on success, error code otherwise.
 */
int postgis_set_database(postgis_context_def *context, const char* database_name);

#endif /* POSTGIS_SET_SCHEMA_H */
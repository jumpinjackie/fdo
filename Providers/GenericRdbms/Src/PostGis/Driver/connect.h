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

#ifndef POSTGIS_CONNECT_H
#define POSTGIS_CONNECT_H

#include "local.h"

#define POSTGIS_TEMPLATE_DBNAME "template_postgis"

/**
 * Open connection with PostGIS data store.
 *
 * @param context [in] - pointer to PostGIS session context.
 * @param connect_string [in] - database connection string, formatted as database@host:port.
 * @param user [in] - PostGIS user name to connect as.
 * @param pswd [in] - password to be used if the server demands password authentication.
 * @param vendor_data [out] - buffer for vendor-specific data.
 * @param connect_id [out] - numeric identifier of established connection,
 *  after successful connect.
 * @return Error code with operation state.
 */
int postgis_connect (
    postgis_context_def *context,
    char  *connect_string,
    char  *user,
    char  *pswd,
    char **vendor_data,
    int   *connect_id);

#endif /* POSTGIS_CONNECT_H */

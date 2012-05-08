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

#ifndef POSTGIS_PGCONN_STATUS_H
#define POSTGIS_PGCONN_STATUS_H

#include <libpq-fe.h>

/**
 * Checks PostgreSQL connection status.
 * This function tests passed pointer to PGconn and translates it's
 * status to RDBI code.
 * If connection is not established, the function also tries to reset connetion.
 *
 * @param conn [in] - pointer to PGconn handler.
 * @return Returns RDBI_SUCCESS if context connection is established,
 * RDBI_NOT_CONNECTED otherwise or RDBI_GENERIC_ERROR if error occured.
 */
int postgis_pgconn_status (PGconn* conn);

#endif /* POSTGIS_PGCONN_STATUS_H */

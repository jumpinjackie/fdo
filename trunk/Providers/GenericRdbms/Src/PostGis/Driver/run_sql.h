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

#ifndef POSTGIS_RUN_SQL_H
#define POSTGIS_RUN_SQL_H

#include "local.h"

/**
 * Parses and executes a SQl statement in one call.
 *
 * It is used to run SQL statement in one shot without the prepare, describe, bind
 * and execute steps. Needed for certain statements such as CREATE DATABASE,
 * DROP DATABASE, etc. that cannot be executed as prepared in PostgreSQL.
 *
 * @param context [in] - pointer to current PostGIS session context.
 * @param sql [in] - string containing SQL statement to execute.
 * @param isddl [in] - flag indicating if the SQL statement is DDL (isddl>1) or not.
 * @param rows_processed [out] - the number of rows processed by the statement.
 * Can be NULL, in which case this parameter is ignored.
 * @return Returns an rdbi status code from Inc/rdbi.h. 
 */
int postgis_run_sql (
    postgis_context_def *context,
    char* sql,
	int isddl,
    int* rows_processed
);

#endif /* POSTGIS_RUN_SQL_H */

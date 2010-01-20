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

#ifndef POSTGIS_SQL_H
#define POSTGIS_SQL_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

/**
 * Run PostGIS-specific SQL parsing and statement instantiation.
 *
 * @param context [in] - pointer to current PostGIS session context.
 * @param cursor [in] - pointer to PostGIS-specific cursor area.
 * @param sql [in] - string with SQL statement.
 * @param defer [in] -
 * @param verb [in] - SQL statement verb.
 * @param ptree [in] - pointer to parse tree, may be NULL.
 * @param cursor_coc [in] - 
 * @return Rdbi error code on failure, RDBI_SUCCESS otherwise.
 */
int postgis_sql (
    postgis_context_def *context,
    char *cursor,
    char *sql,
    int defer,
    char *verb,
    void *ptree, /* dbi_prse_tree_def */
    char *cursor_coc);

#endif /* POSTGIS_SQL_H */

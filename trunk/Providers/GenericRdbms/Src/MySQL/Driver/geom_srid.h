/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef GEOM_SRID_H
#define GEOM_SRID_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

int mysql_geom_srid_set ( 
	mysql_context_def	*context,
    char   *cursor,                     /* cursor associated with SQL stmnt */
	char	*geom_col_name,
    long srid ) ;

#endif // GEOM_SRID_H

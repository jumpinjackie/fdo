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

#ifndef POSTGIS_VNDR_NAME_H
#define POSTGIS_VNDR_NAME_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

/**
 * Get vendor name for current session context.
 * Vendor name for PostGIS provider is "PostGIS".
 *
 * @param context [in] - pointer to PostGIS session context.
 * @return String containing vendor name.
 */
char *postgis_vndr_name(postgis_context_def *context);

#endif /* POSTGIS_VNDR_NAME_H */

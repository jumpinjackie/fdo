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

#ifndef POSTGIS_VNDR_INFO_H
#define POSTGIS_VNDR_INFO_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"
#include <Inc/Rdbi/vndr_info.h>

/**
 * Fetch vendor information.
 * 
 * @param context - pointer to current PostGIS session context structure
 * @param vndr_info - pointer to vendor information structure
 *                    that will be filled by function.
 * @return RDBI_SUCCESS code on success, error code otherwise.
 */
int postgis_vndr_info (postgis_context_def *context, rdbi_vndr_info_def *vndr_info);

#endif /* POSTGIS_VNDR_INFO_H */

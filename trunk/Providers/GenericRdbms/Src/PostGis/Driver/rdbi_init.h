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

#ifndef POSTGIS_RDBI_INIT_H
#define POSTGIS_RDBI_INIT_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <Inc/Rdbi/methods.h>   /* rdbi methods table */

/**
 * Initializes RDBI context and functions registry of PostGIS driver.
 *
 * @param contextp [out] - pointer to postgis_context_def structure.
 * @param methods [out] - pointer to rdbi_methods_def structure.
 * @return Error code indicating context status.
 */
int postgis_rdbi_init (void **contextp, rdbi_methods methods);

#endif /* POSTGIS_RDBI_INIT_H */

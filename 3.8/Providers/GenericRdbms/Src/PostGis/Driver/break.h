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

#ifndef POSTGIS_BREAK_H
#define POSTGIS_BREAK_H

#include "local.h"

/**
 * Cancels any operation that is currently executing.
 * THIS FUNCTION IS NOT USED IN CURRENT PostGIS RDBI DRIVER!
 * See assert() in the function body.
 *
 * @param context [in] - pointer to PostGIS session context.
 * @return Always returns RDBI_GENERIC_ERROR error code.
 *
 * \sa rdbi_break
 */
int postgis_do_break(postgis_context_def* context);

#endif /* POSTGIS_BREAK_H */

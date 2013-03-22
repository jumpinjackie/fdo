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

#ifndef POSTGIS_SET_ERR_MSG_H
#define POSTGIS_SET_ERR_MSG_H

#include "local.h"

/**
 * Sets passed message as a last eror message for current context.
 * This function is used internally by the PostGIS RDBI Driver only,
 * it's not dispatched by the RDBI layer.
 *
 * @param context [out] - pointer to current PostGIS session context.
 *  The function updates passed context by setting last error message.
 * @param msg [in] - string with message being assigned to the context.
 * @return Returns an rdbi status code from Inc/rdbi.h. 
 */
int postgis_set_err_msg (
    postgis_context_def *context,
    const char* msg);

#endif /* POSTGIS_SET_ERR_MSG_H */

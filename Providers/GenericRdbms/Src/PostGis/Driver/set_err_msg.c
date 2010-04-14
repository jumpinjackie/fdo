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

#include "stdafx.h"
#include "set_err_msg.h"

#include <string.h> /* memset(), strncpy() */
#include <assert.h>

int postgis_set_err_msg (
    postgis_context_def *context,
    const char* msg
)
{
    int ret = RDBI_GENERIC_ERROR;

    if (NULL != context && NULL != msg)
    {
        memset(context->postgis_last_err_msg, '\0', RDBI_MSG_SIZE);
        strncpy(context->postgis_last_err_msg, msg, RDBI_MSG_SIZE);
        context->postgis_last_err_msg[RDBI_MSG_SIZE - 1] = '\0';

        ret = RDBI_SUCCESS;
    }

    return ret;
}

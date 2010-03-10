/*
 * Copyright (C) 2010 Refractions Research, Inc. 
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

#ifndef POSTGIS_GET_NEXT_SEQ_H
#define POSTGIS_GET_NEXT_SEQ_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"

/** 
 * Gets the next value from a sequence
 *
 * @param context [in] - pointer to PostGIS session context.
 * @param sequence_name [in] - the sequence
 * @param id [out] - next value from the sequence.
 * @return RDBI error code of operation state.
 */
int postgis_get_next_seq(
    postgis_context_def* context,
    const char* sequence_name,
    long* id);

#endif /* POSTGIS_GET_NEXT_SEQ_H */

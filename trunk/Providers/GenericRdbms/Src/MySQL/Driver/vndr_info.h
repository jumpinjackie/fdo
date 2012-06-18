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

#ifndef VNDR_INFO_H
#define VNDR_INFO_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "local.h"
#include <Inc/Rdbi/vndr_info.h>

int mysql_vndr_info (mysql_context_def *context, rdbi_vndr_info_def *vndr_info);

#endif // VNDR_INFO_H

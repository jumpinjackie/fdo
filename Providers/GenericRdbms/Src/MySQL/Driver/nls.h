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

#ifndef MYSQL_NLS_H
#define MYSQL_NLS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

// Constructs a message indicating that the current LibMySQL is too old for FDO
const wchar_t* mysql_nls_client_version( unsigned long client_version );

// Constructs a message indicating that the connected MySQL database is too old for FDO
const wchar_t* mysql_nls_server_version( unsigned long client_version );

const wchar_t* mysql_nls_msg_get(int msg_num, char* default_msg,  ...);

#endif // MYSQL_NLS_H

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

#ifndef FDOWFSPROVIDER_H
#define FDOWFSPROVIDER_H

// General descriptive items of WFS Provier:
#define WFS_PROVIDER_NAME					L"OSGeo.WFS.3.5"
#define WFS_PROVIDER_DEFAULT_DISPLAY_NAME	"OSGeo FDO Provider for WFS"
#define WFS_PROVIDER_DEFAULT_DESCRIPTION	"Read access to OGC WFS-based data store."
#define WFS_PROVIDER_VERSION				L"3.5.0.0"
#define WFS_FDO_VERSION						L"3.5.0.0"

// Connection property strings:
const wchar_t WFS_CONNECTIONPROPERTY_SERVER[] = L"Server";
const wchar_t WFS_CONNECTIONPROPERTY_INSTANCE[] = L"Instance";
const wchar_t WFS_CONNECTIONPROPERTY_DATASTORE[] = L"Datastore";
const wchar_t WFS_CONNECTIONPROPERTY_USERNAME[] = L"Username";
const wchar_t WFS_CONNECTIONPROPERTY_PASSWORD[] = L"Password";
const wchar_t WFS_CONNECTIONPROPERTY_DELIMITER[] = L";";
const wchar_t WFS_CONNECTIONPROPERTY_SEPARATOR[] = L"=";

#endif // FDOWFSPROVIDER_H

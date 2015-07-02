/*
 * Copyright (C) 2007  Haris Kurtagic
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
 */

#ifdef _WIN32
#pragma once
#endif

#ifndef _c_KgMssOverXmlGlobals_h
#define _c_KgMssOverXmlGlobals_h

namespace c_KgMssOverXmlGlobals
{
    static FdoString* g_KgOraSchemaMapping                    = L"SchemaMapping";
    static FdoString* g_KgOraClassDefinitionElement           = L"ComplexType";
    static FdoString* g_KgOraFullTableNameAttribute           = L"FullTableName";
    static FdoString* g_KgOraPropertyElement                  = L"Element";
    static FdoString* g_KgOraColumnElement                    = L"Column";
    static FdoString* g_KgOraXmlnsValue                       = L"http://fdooracle.osgeo.org/schemas";
};

#endif // _FDOKGORAXMLGLOBALS__H_



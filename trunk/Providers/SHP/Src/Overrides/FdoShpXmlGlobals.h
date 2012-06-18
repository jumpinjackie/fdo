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
 */

#ifdef _WIN32
#pragma once
#endif

#ifndef _FDOSHPXMLGLOBALS__H_
#define _FDOSHPXMLGLOBALS__H_

namespace FdoShpOvXmlGlobals
{
    static FdoString* g_SHPSchemaMapping                    = L"SchemaMapping";
    static FdoString* g_SHPClassDefinitionElement           = L"complexType";
    static FdoString* g_SHPShapeFileAttribute               = L"shapeFile";
    static FdoString* g_SHPPropertyElement                  = L"element";
    static FdoString* g_SHPColumnElement                    = L"Column";
    static FdoString* g_SHPXmlnsValue                       = L"http://fdoshp.osgeo.org/schemas";
};

#endif // _FDOSHPXMLGLOBALS__H_



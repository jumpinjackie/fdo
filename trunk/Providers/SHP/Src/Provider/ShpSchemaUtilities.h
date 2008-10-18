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

#ifndef SHPSCHEMAUTILITIES_H
#define SHPSCHEMAUTILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32


class ShpSchemaUtilities
{
public:
    // Convert column type to/from logical property type:
    static FdoDataType DbfTypeToFdoType (int type);
    static eDBFColumnType FdoTypeToDbfType (FdoDataType type);

    // Misc conversion functions:
    static eShapeTypes FdoGeometryToShapeType (FdoGeometryType* geomTypes, FdoInt32 geomTypeCount, bool has_elevation, bool has_measure);

    // Returns the requested logical class definition, paring it down to the given property list
    static FdoClassDefinition* GetLogicalClassDefinition (ShpConnection* connection, FdoString* logicalClassName, FdoIdentifierCollection* selectedProperties);

    // Returns the request LogicalPhysical class, by logical class name:
    static ShpLpClassDefinition* GetLpClassDefinition(ShpConnection* connection, FdoString* logicalClassName);

    // Returns the physical column name corresponding to the requested logical property name:
    static FdoString* GetPhysicalColumnName(ShpConnection* connection, FdoString* logicalClassName, FdoString* logicalPropertyName);

    // returns the logical name for "FeatId"
    static FdoStringP GetIdentityPropertyName (ShpConnection* connection, FdoString* class_name);

};

#endif // SHPSCHEMAUTILITIES_H


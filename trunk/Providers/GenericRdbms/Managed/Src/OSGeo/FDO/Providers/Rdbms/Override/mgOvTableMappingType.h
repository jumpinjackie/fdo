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

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

///<summary>OvTableMappingType is an enumeration of the possible mapping types</summary>
/// <param name="OvTableMappingType_Default"> Defaults to Concrete table. </param>
/// <param name="OvTableMappingType_ClassTable"> only explicit properties are stored in the class
/// table. Inherited properties are stored in the base class's table. This means that objects in this class
/// will have their properties in multiple tables. These tables will be related by their primary key columns.
/// Most providers, including all Autodesk providers, do not currently support class table mapping. These providers
/// revert to using ConcreteTable mapping if ClassTable mapping is specified.
/// </param>
/// <param name="OvTableMappingType_ConcreteTable"> all class properties ( explicit and inherited )
/// are stored in the class table. The class table can be the same or different from the class table for its
/// base class. </param>
/// <param name="OvTableMappingType_BaseTable"> all explicit class properties are store in the table
/// for the base class. It is an error if this option is specified for a class that does not have a base
/// class. It is also an error if the class also contains a Table object, since this class won't have its
/// own table. For most cases, Base Table is equivalent to the Class Table option with table name set to the
/// base class table name. The exception is when the base class table name is adjusted. In this case, the
/// Base Table option allows the class properties to be stored in the base class table without the application
/// having to guess how this table name will be adjusted. </param>
public __value enum OvTableMappingType
{
    OvTableMappingType_Default = FdoSmOvTableMappingType_Default,
    OvTableMappingType_ClassTable = FdoSmOvTableMappingType_ClassTable,
    OvTableMappingType_ConcreteTable = FdoSmOvTableMappingType_ConcreteTable,
    OvTableMappingType_BaseTable = FdoSmOvTableMappingType_BaseTable
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE


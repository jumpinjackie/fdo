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

class FdoShpOvClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE
public __gc class PropertyDefinition;
public __gc class PropertyDefinitionCollection;

/// <summary> 
/// The FdoShpOvClassDefinition class defines a raster image which contains a name 
/// and the extents of the raster image.
/// </summary>
public __gc class ClassDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping
{
public:
	/// <summary>Constructs a new empty class definition.</summary>
	ClassDefinition();

	__property NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinitionCollection* get_Properties();
    
    __property System::String* get_ShapeFile();

    __property System::Void set_ShapeFile(System::String* location);

    // Helper method to find a property mapping by its column name:
    NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition* FindByColumnName(System::String* columnName);

public private:
	ClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoShpOvClassDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE

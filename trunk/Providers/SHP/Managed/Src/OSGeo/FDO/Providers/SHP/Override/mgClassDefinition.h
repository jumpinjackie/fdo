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
ref class PropertyDefinition;
ref class PropertyDefinitionCollection;

/// <summary> 
/// The FdoShpOvClassDefinition class defines a raster image which contains a name 
/// and the extents of the raster image.
/// </summary>
public ref class ClassDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping
{
public:
	/// <summary>Constructs a new empty class definition.</summary>
	ClassDefinition();

    property NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinitionCollection^ Properties
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinitionCollection^ get();
    }    
    
    property System::String^ ShapeFile
    {
        System::String^ get();
        System::Void set(System::String^ location);
    }

    // Helper method to find a property mapping by its column name:
    NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE::PropertyDefinition^ FindByColumnName(System::String^ columnName);

internal:
	ClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoShpOvClassDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE

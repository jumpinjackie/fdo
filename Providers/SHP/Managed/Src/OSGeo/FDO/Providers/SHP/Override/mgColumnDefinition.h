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

class FdoShpOvColumnDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE

/// <summary> 
/// The FdoShpOvColumnDefinition class defines a raster image which contains a name 
/// and the extents of the raster image.
/// </summary>
public ref class ColumnDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
	/// <summary>Constructs a new empty class definition.</summary>
	ColumnDefinition();

internal:
	ColumnDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoShpOvColumnDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_SHP_OVERRIDE

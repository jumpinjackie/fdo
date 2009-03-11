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
*
*/

#pragma once

class FdoWmsOvClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE
ref class OvRasterDefinition;

/// <summary> 
/// The FdoWmsOvOvClassDefinition class defines mapping overrides for a 
/// WMS feature class that exposes WMS layers as an FDO class. The 
/// properties defined here will be used in the WMS Getap operation 
/// when querying data from the WMS server.
/// </summary>
public ref class OvClassDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping
{
public:
	/// <summary>Constructs a new empty WMS physical mapping class definition.</summary>
	OvClassDefinition();
    
    /// <summary>Gets the physical overrides for a raster property definition.</summary>
    /// <returns>Returns the raster definition. </returns> 
    /// <summary>Sets the physical overrides for a raster property definition.</summary>	
    /// <param name="rasterDefinition">Input raster definition</param>
	/// <returns>Returns nothing</returns> 
    property NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition^ RasterDefinition
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvRasterDefinition^ rasterDefinition);
    }

internal:
	OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoWmsOvClassDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE

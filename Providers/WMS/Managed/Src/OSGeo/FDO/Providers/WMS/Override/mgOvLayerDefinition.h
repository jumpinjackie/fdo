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

class FdoWmsOvLayerDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE
ref class OvStyleDefinition;

/// <summary> 
/// The FdoWmsOvLayerDefinition class defines a WMS style that will be used to stylize a WMS layer.
/// </summary>
public ref class OvLayerDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
	/// <summary>Constructs a new empty layer definition.</summary>
	OvLayerDefinition();

    /// <summary>Gets the WMS style that will be used to render a WMS 
    /// layer in a composit WMS image.</summary>
    /// <returns>Returns the style definition.</returns> 
    /// <remarks>If the server advertises several styles for a layer, and the style 
    /// definition is either set to the default style or not specified at all, the 
    /// choice of which style to use as default is at the discretion of the server.</remarks>
    /// <summary>Sets the WMS style that will be used to render a WMS 
    /// layer in a composit WMS image.</summary>
    /// <returns>Returns nothing.</returns> 
    /// <remarks>If the server advertises several styles for a layer, and the style 
    /// definition is either set to the default style or not specified at all, the 
    /// choice of which style to use as default is at the discretion of the server.</remarks>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition^ Style
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvStyleDefinition^ value);
    }

internal:
	OvLayerDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoWmsOvLayerDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE

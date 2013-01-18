/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

class FdoWmsOvRasterDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE
ref class OvLayerCollection;

/// <summary> 
/// The FdoWmsOvRasterDefinition class defines the physical overrides for a raster property in a WMS FDO schema.
/// </summary>
public ref class OvRasterDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
	/// <summary>Constructs a new Raster Definition.</summary>
	OvRasterDefinition();

    /// <summary>Gets the format type in which the WMS image will be generated.</summary>
    /// <returns>Returns the WMS format type.</returns> 
    /// <remarks>Allowed map formats are "picture" formats . Picture formats constitute 
    /// a rectangular pixel array of fixed size. Picture formats include file types such 
    /// as Portable Network Graphics (PNG), Joint Photographics Expert Group (JPEG) 
    /// and file types such as Tagged Image File Format (TIFF).</remarks> 
    /// <summary>Sets the format type in which the WMS image will be generated.</summary>
    /// <returns>Returns nothing.</returns> 
    /// <remarks>Allowed map formats are "picture" formats . Picture formats constitute 
    /// a rectangular pixel array of fixed size. Picture formats include file types such 
    /// as Portable Network Graphics (PNG), Joint Photographics Expert Group (JPEG) 
    /// and file types such as Tagged Image File Format (TIFF).</remarks> 
    property String^ ImageFormat
    {
        String^ get();
        System::Void set(String^ value);
    }

    /// <summary>Gets the transparency state.</summary>
    /// <returns>Returns a FdoBoolean indicating the state of the transparency flag.</returns> 
    /// <remarks>The transparency state specifies whether the WMS map background is to  
    /// be made transparent or not. The default value is false. The ability to return 
    /// an image drawn with transparent pixels allows results of different Map requests 
    /// to be overlaid, producing a composite map. </remarks>
    /// <summary>Sets the transparency state.</summary>
    /// <returns>Returns nothing.</returns> 
    /// <remarks>The transparency state specifies whether the WMS map background is to  
    /// be made transparent or not. The default value is false. The ability to return 
    /// an image drawn with transparent pixels allows results of different Map requests 
    /// to be overlaid, producing a composite map. </remarks>
    property System::Boolean Transparent
    {
        System::Boolean get();
        System::Void set(System::Boolean transparent);
    }

    /// <summary> Gets the tile caching option value. </summary>
    /// <returns> Returns a FdoBoolean indicating whether using tile cache or not. </returns> 
    /// <remarks> This option specifies whether using the tile caching method to query the raster data.
    /// The default value is true.</remarks> 
    /// <summary> Sets the tile caching option value. </summary>
    /// <returns> Returns nothing. </returns> 
    /// <remarks> This option specifies whether using the tile caching method to query the raster data.
    /// The default value is true.</remarks> 
    property System::Boolean UseTileCache
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// <summary>Gets the background color.</summary>
    /// <returns>Returns a System::String indicating the background color 
    /// to be used when rendering the WMS image.</returns> 
    /// <remarks>The background color is a string that specifies the color to be used 
    /// as the background pixels of the map. The general format of the background color 
    /// is a hexadecimal encoding of an RGB value where two hexadecimal characters 
    /// are used for each of Red, Green, and Blue colour values. The values can range 
    /// between 00 and FF for each (0 and 255, base 10). The format is 0xRRGGBB; either 
    /// upper or lower case characters are allowed for RR, GG, and BB values. 
    /// The "0x" prefix shall have a lower case ‘x? The default value is 0xFFFFFF 
    /// (corresponding to the colour white).</remarks>
    /// <summary>Sets the background color.</summary>
    /// <returns>Returns nothing.</returns> 
    /// <remarks>The background color is a string that specifies the color to be used 
    /// as the background pixels of the map. The general format of the background color 
    /// is a hexadecimal encoding of an RGB value where two hexadecimal characters 
    /// are used for each of Red, Green, and Blue colour values. The values can range 
    /// between 00 and FF for each (0 and 255, base 10). The format is 0xRRGGBB; either 
    /// upper or lower case characters are allowed for RR, GG, and BB values. 
    /// The "0x" prefix shall have a lower case ‘x? The default value is 0xFFFFFF 
    /// (corresponding to the colour white).</remarks>
    property String^ BackgroundColor
    {
        String^ get();
        System::Void set(String^ value);
    }

    /// <summary>Gets the time dimension of the requested WMS map.</summary>
    /// <returns>Returns a System::String indicating the time dimension of the 
    /// requested WMS image.</returns> 
    /// <remarks>If a WMS map has a Time dimension defined, then requests may 
    /// include the parameter TIME=value. For the TIME parameter, the special keyword 
    /// 'current' may be used if the WMS Dimension:Name="time" service metadata 
    /// element includes a nonzero value for the 'current' attribute. The expression 
    /// "TIME=current" means send the most current data available. The expression 
    /// "TIME=start_time/current" means send data from start_time up to the most 
    /// current data available.</remarks>
    /// <summary>Sets the time dimension of the requested WMS map.</summary>
    /// <returns>Returns nothing .</returns> 
    /// <remarks>If a WMS map has a Time dimension defined, then requests may 
    /// include the parameter TIME=value. For the TIME parameter, the special keyword 
    /// 'current' may be used if the WMS Dimension:Name="time" service metadata 
    /// element includes a nonzero value for the 'current' attribute. The expression 
    /// "TIME=current" means send the most current data available. The expression 
    /// "TIME=start_time/current" means send data from start_time up to the most 
    /// current data available.</remarks>
    property String^ TimeDimension
    {
        String^ get();
        System::Void set(String^ value);
    }

    /// <summary>Gets the elevation dimension of the requested WMS map.</summary>
    /// <returns>Returns a System::String indicating the elevation dimension 
    /// of the requested WMS image.</returns> 
    /// <remarks>If a WMS Map has a Elevation dimension defined, then in 
    /// order to retrieve the map in the required dimension, 
    /// the dimension property may set to 'ELEVATION=value'.</remarks>
    /// <summary>Sets the time dimension of the requested WMS map.</summary>
    /// <returns>Returns nothing.</returns> 
    /// <returns>Returns a System::String indicating the elevation dimension 
    /// of the requested WMS image.</returns> 
    /// <remarks>If a WMS Map has a Elevation dimension defined, then in 
    /// order to retrieve the map in the required dimension, 
    /// the dimension property may set to 'ELEVATION=value'.</remarks>
    property String^ ElevationDimension
    {
        String^ get();
        System::Void set(String^ dimension);
    }

    /// <summary>Gets the spatial context in which the requested 
    /// WMS map will be returned.</summary>
    /// <returns>Returns the WMS Provider's spatial context name.</returns> 
    /// <remarks>The same spatial context name applies to all layers 
    /// in a single WMS class definition.</remarks>
    /// <summary>Sets the spatial context in which the requested 
    /// WMS map will be returned.</summary>
    /// <returns>Returns nothing.</returns> 
    /// <remarks>The same spatial context name applies to all layers 
    /// in a single WMS class definition.</remarks>
    property String^ SpatialContextName
    {
        String^ get();
        System::Void set(String^ value);
    }

    /// <summary>Gets the WMS layers that will be used to construct 
    /// the composite WMS image.</summary>
    /// <returns>Returns the collection of WMS layers associated to the logical 
    /// FDO class definition.</returns> 
    /// <remarks>A WMS shall render the requested layers by drawing 
    /// the first map in the list bottommost, the next one over that, 
    /// and so on.</remarks>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection^ Layers
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE::OvLayerCollection^ get();
    }

internal:
	OvRasterDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoWmsOvRasterDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE

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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_RASTER
/// \endcond

/// \ingroup (OSGeoFDORaster)
/// \brief
/// The RasterDataModelType enumeration specifies the type of
/// data within each pixel. In all cases the data is 'packed', that is,
/// adjacent pixels are contiguous in memory with no unused bits between
/// them, except at the end of a row, where the row is padded to a
/// byte boundary. For example, four bit grayscale data (BitsPerPixel=4)
/// would be packed two pixels to a byte, and a row would be padded with
/// four bits if the image width was odd. Tri-colour or four channel data
/// is also organized adjacently in memory, with no intervening padding
/// except at the ends of rows. For example, eight bit RGB (BitsPerPixel=24)
/// occupies three bytes per pixel, and would never need padding at the
/// end of a row. Multiband data (i.e. RGB) has the same number of bits
/// allocated to each band, so for example RGBT data with eight bits for red,
/// would also have eight bits for each of the three other bands totaling
/// thirty-two bits (BitsPerPixel=32).
public enum class RasterDataModelType
{
    /// Data is organized in an unknown or provider specific manner.
	RasterDataModelType_Unknown = FdoRasterDataModelType_Unknown,

	/// Data is rectangular grid, floating point data model.
    RasterDataModelType_Data = FdoRasterDataModelType_Data,

    /// Data is one bit (black/white - bitonal) pixels.
	RasterDataModelType_Bitonal = FdoRasterDataModelType_Bitonal,

    /// Data is monochrome (black->gray->white) pixels.
    /// It is necessary to know the BitsPerPixel value to interpret the data.
	RasterDataModelType_Gray = FdoRasterDataModelType_Gray,

    /// Data is red/green/blue (in that order) pixels.
    /// It is necessary to know the BitsPerPixel value to interpret the data.
	RasterDataModelType_RGB = FdoRasterDataModelType_RGB,

    /// Data is red/green/blue/alpha (in that order) pixels; RGB with transparency.
    /// It is necessary to know the BitsPerPixel value to interpret the data.
	RasterDataModelType_RGBA = FdoRasterDataModelType_RGBA,

    /// Data is monochrome but backed by a pallette (value->pallette->colour) pixels.
    /// It is necessary to know the BitsPerPixel value to interpret the data.
	RasterDataModelType_Palette = FdoRasterDataModelType_Palette
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_RASTER
/// \endcond

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_RASTER

/// \ingroup (enums)
/// \brief
/// The RasterDataOrganization enumeration specifies the way
/// pixel data is interleaved. Data is stored/retrieved in left to right,
/// and secondarily top to bottom order within the interleave order.
/// Tiles are also stored/retrieved in left to right, top to bottom order.
/// For tiled data, data for one tile is completely stored/retrieved before
/// the next tile, so for example, with 'RasterDataOrganization_Image'
/// organization of RGB data with 24 bits per pixel, the TileSizeX by
/// TileSizeY 8 bit data values for the red channel are stored first
/// followed by the same number of green bytes, followed by the same
/// number of blue bytes. This organization dependant storage should be
/// considered when 'skipping' to the middle of an image.
public __value enum RasterDataOrganization
{
    /// Data is interleaved by pixel, with all data for a pixel contiguous in memory.
	RasterDataOrganization_Pixel = FdoRasterDataOrganization_Pixel,

    /// Data is interleaved by row, with all data of a channel for one row
    /// contiguous in memory, followed by a rows worth of the next channel, etc.
	RasterDataOrganization_Row = FdoRasterDataOrganization_Row,

    /// Data is interleaved by image, with all data of a channel for a complete
    /// image contiguous in memory, followed by a complete image of the next
    /// channel, etc.
	RasterDataOrganization_Image = FdoRasterDataOrganization_Image
};

END_NAMESPACE_OSGEO_FDO_RASTER



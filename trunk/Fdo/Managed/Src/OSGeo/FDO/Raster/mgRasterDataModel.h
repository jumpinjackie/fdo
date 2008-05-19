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

#include "FDO\Raster\mgRasterDataModelType.h"
#include "FDO\Raster\mgRasterDataType.h"
#include "FDO\Raster\mgRasterDataOrganization.h"

class FdoRasterDataModel;

BEGIN_NAMESPACE_OSGEO_FDO_RASTER

/// \ingroup (OSGeoFDORaster)
/// \brief
/// The RasterDataModel specifies the data type and organization
/// of raster data retrieved and stored. Using this class and the image
/// extents in width and length, the binary format of the image data returned
/// by and accepted by the IStreamReader class can be interpreted.
public __gc class RasterDataModel: public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Creates a new datamodel.
    /// 
    /// \return
    /// Returns a bitonal 256x256 tiled data model.
    /// 
	RasterDataModel();

    /// \brief
    /// Gets the type of data available from this datamodel.
    /// 
    /// \return
    /// Returns the type of data available with this data model.
    /// 
	__property NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType get_DataModelType ();

    /// \brief
    /// Sets the type of data for this datamodel.
    /// 
    /// \param type 
    /// The type of data for this data model.
    /// 
	__property System ::Void set_DataModelType (NAMESPACE_OSGEO_FDO_RASTER::RasterDataModelType type);

    /// \brief
    /// Get the number of bits per pixel.
    /// 
    /// \return
    /// Returns the number of bits for each pixel. For multi-channel
    /// data the bits per channel will be this value devided by the numer of
    /// channels. For example, RGB data has three channels, so if this
    /// method returns twelve, each channel is four bits.
    /// 
	__property System ::Int32 get_BitsPerPixel ();

    /// \brief
    /// Set the number of bits per pixel.
    /// 
    /// \param bitsPerPixel 
    /// The number of bits per pixel desired.
    /// Values of 1, 4, 8, 16, 24, 32, 48 and 64 bits per channel may
    /// be supported. Others values (i.e. indivisible by the number of channels)
    /// are likely to raise a Exception.
    /// 
	__property System ::Void set_BitsPerPixel (System::Int32 bitsPerPixel);

    /// \brief
    /// Get the data interleave organization.
    /// 
    /// \return
    /// Returns the way data is interleaved.
    /// 
	__property NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization get_Organization();

    /// \brief
    /// Set the data interleave organization.
    /// 
    /// \param organization 
    /// Specifies the way data is to be interleaved.
    /// 
	__property System::Void set_Organization (NAMESPACE_OSGEO_FDO_RASTER::RasterDataOrganization organization);

    /// \brief
    /// Get the tile size in the horizontal direction (number of columns).
    /// 
    /// \return
    /// The number of columns in a tile.
    /// 
	__property System::Int32 get_TileSizeX ();

    /// \brief
    /// Set the tile size in the horizontal direction (number of columns).
    /// 
    /// \param sizex 
    /// The number of columns in a tile.
    /// 
	__property System::Void set_TileSizeX (System::Int32 sizex);

    /// \brief
    /// Get the tile size in the vertical direction (number of rows).
    /// 
    /// \return
    /// The number of rows in a tile.
    /// 
	__property System::Int32 get_TileSizeY ();

    /// \brief
    /// Set the tile size in the vertical direction (number of rows).
    /// Values of 64, 128, 256, 512, 1024 and the image extents in width and
    /// length may be supported. When the tile sizes are equal to the image
    /// extents, the image is monolithic. Rectangular tile sizes, combining
    /// different values for X and Y, may be supported.
    /// 
    /// \param sizey 
    /// The number of rows in a tile.
    /// 
	__property System::Void set_TileSizeY (System::Int32 sizey);

    /// \brief
    /// Gets the data type.
    /// 
    /// \return
    /// Returns the data type of the raster.
    /// 
	__property NAMESPACE_OSGEO_FDO_RASTER::RasterDataType get_DataType();

    /// \brief
    /// Sets the data type.
    /// 
    /// \param dataType 
    /// Specifies the data type the raster data is to be returned in.
    /// 
	__property System::Void set_DataType (NAMESPACE_OSGEO_FDO_RASTER::RasterDataType dataType);

    /// \brief
    /// Constructs a RasterDataModel object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	RasterDataModel(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	inline FdoRasterDataModel* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_RASTER



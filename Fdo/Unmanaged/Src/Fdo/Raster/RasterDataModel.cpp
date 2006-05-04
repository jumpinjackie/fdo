// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Std.h>
#include <Fdo/Raster/RasterDataModel.h>

FdoRasterDataModel::FdoRasterDataModel () :
    m_Type (FdoRasterDataModelType_Bitonal),
    m_BitsPerPixel (1),
    m_Organization (FdoRasterDataOrganization_Pixel),
    m_TileX (256),
    m_TileY (256),
    m_DataType(FdoRasterDataType_Unknown)
{
}

/// <summary>Creates a new datamodel.</summary>
/// <returns>Returns a bitonal 256x256 tiled data model.</returns>
FdoRasterDataModel* FdoRasterDataModel::Create ()
{
    return (new FdoRasterDataModel ());
}

/// <summary>Gets the type of data available from this datamodel.</summary>
/// <returns>Returns the type of data available with this data model.</returns>
FdoRasterDataModelType FdoRasterDataModel::GetDataModelType ()
{
    return (m_Type);
}

/// <summary>Sets the type of data for this datamodel.</summary>
/// <param name="type">The type of data for this data model.</param>
void FdoRasterDataModel::SetDataModelType (FdoRasterDataModelType type)
{
    m_Type = type;
}

/// <summary>Get the number of bits per pixel.</summary>
/// <returns>Returns the number of bits for each pixel. For multi-channel
/// data the bits per channel will be this value devided by the numer of
/// channels. For example, RGB data has three channels, so if this
/// method returns twelve, each channel is four bits.</returns>
FdoInt32 FdoRasterDataModel::GetBitsPerPixel ()
{
    return (m_BitsPerPixel);
}

/// <summary>Set the number of bits per pixel.</summary>
/// <param name="bpp">The number of bits per pixel desired.
/// Values of 1, 4, 8, 16, 24, 32, 48 and 64 bits per channel may
/// be supported. Others values (i.e. indivisible by the number of channels)
/// are likely to raise a FdoException.</param>
void FdoRasterDataModel::SetBitsPerPixel (FdoInt32 bpp)
{
    m_BitsPerPixel = bpp;
}

/// <summary>Get the data interleave organization.</summary>
/// <returns>Returns the way data is interleaved.</returns>
FdoRasterDataOrganization FdoRasterDataModel::GetOrganization ()
{
    return (m_Organization);
}

/// <summary>Set the data interleave organization.</summary>
/// <param name="organization">Specifies the way data is to be interleaved.</param>
void FdoRasterDataModel::SetOrganization (FdoRasterDataOrganization organization)
{
    m_Organization = organization;
}

/// <summary>Get the tile size in the horizontal direction (number of columns).</summary>
/// <returns>The number of columns in a tile.</returns>
FdoInt32 FdoRasterDataModel::GetTileSizeX ()
{
    return (m_TileX);
}

/// <summary>Set the tile size in the horizontal direction (number of columns).</summary>
/// <param name="sizex">The number of columns in a tile.</param>
void FdoRasterDataModel::SetTileSizeX (FdoInt32 sizex)
{
    m_TileX = sizex;
}


/// <summary>Get the tile size in the vertical direction (number of rows).</summary>
/// <returns>The number of rows in a tile.</returns>
FdoInt32 FdoRasterDataModel::GetTileSizeY ()
{
    return (m_TileY);
}

/// <summary>Set the tile size in the vertical direction (number of rows).
/// Values of 64, 128, 256, 512, 1024 and the image extents in width and
/// length may be supported. When the tile sizes are equal to the image
/// extents, the image is monolithic. Rectangular tile sizes, combining
/// different values for X and Y, may be supported.</summary>
/// <param name="sizey">The number of rows in a tile.</param>
void FdoRasterDataModel::SetTileSizeY (FdoInt32 sizey)
{
    m_TileY = sizey;
}

/// <summary>Gets the data type.</summary>
/// <returns>Returns the data type of the raster.</returns>
FdoRasterDataType FdoRasterDataModel::GetDataType ()
{
    return (m_DataType);
}


/// <summary>Sets the data type.</summary>
/// <param name="value">Specifies the data type the raster data is to be returned in.</param>
void FdoRasterDataModel::SetDataType (FdoRasterDataType dataType)
{
    m_DataType = dataType;
}

//
// FdoIDisposable interface
//

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void FdoRasterDataModel::Dispose ()
{
    delete this;
}


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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpRaster.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpBandRaster.h"
#include "FdoRfpRaster.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpRasterPropertyDictionary.h"
#include "FdoRfpStreamReaderRGB24.h"
#include "FdoRfpImage.h"
#include "FdoRfpEnvelope.h"
#include <math.h>
#include "FdoRfpRasterUtil.h"
#include "FdoRfpGlobals.h"

FdoRfpRaster::FdoRfpRaster(const FdoPtr<FdoRfpGeoRasterCollection>& geoRasters, FdoRfpRect* clippingBounds) : 
	m_currentBand(0)
{
    if (clippingBounds != NULL)
        m_clippingBounds = new FdoRfpRect(*clippingBounds);

    m_bandRasters = FdoRfpBandRasterCollection::Create();

    _prepareBandRaster(geoRasters, clippingBounds);
}

void FdoRfpRaster::_prepareBandRaster(const FdoPtr<FdoRfpGeoRasterCollection>& geoRasters, FdoRfpRect* clippingBounds)
{
    FdoInt32 i, j, count, maxBandCount = 0;

    // find out the largest band number in the raster collection
    count = geoRasters->GetCount();
    for (i=0; i<count; i++ )
    {
        FdoRfpGeoRasterP geoRaster = geoRasters->GetItem(i);
        if (geoRaster->GetNumberOfBands() > maxBandCount)
            maxBandCount = geoRaster->GetNumberOfBands();
    }

    // set number of bands
    SetNumberOfBands(maxBandCount);

    // create FdoRfpRasterCollection according to maxBandCount
    // for every band, all of the instances for the band are fetched out to compose a FdoRfpBandRaster. 
    for (i=0; i<maxBandCount; i++)
    {
        FdoRfpGeoBandRastersP geoBandRasters = FdoRfpGeoBandRasterCollection::Create();
        for (j=0; j<count; j++)
        {
            FdoRfpGeoRasterP geoRaster = geoRasters->GetItem(j);
            FdoRfpGeoBandRasterP band = geoRaster->GetBand(i);
            if (band != NULL)
                geoBandRasters->Add(band);
        }
        FdoRfpBandRasterP bandRaster = new FdoRfpBandRaster(geoBandRasters->GetItem(0), clippingBounds);
        m_bandRasters->Add(bandRaster);
    }
}

FdoInt32 FdoRfpRaster::GetNumberOfBands ()
{
	return m_bandSize;
}

void FdoRfpRaster::SetNumberOfBands (FdoInt32 value)
{
	m_bandSize = value;
}

/*
	for the clients use the band number starting with 1, and internally it start from 0.
	so there is a little difference. we have to change the number between 
*/
FdoInt32 FdoRfpRaster::GetCurrentBand ()
{
	return m_currentBand+1;
}
void FdoRfpRaster::SetCurrentBand (FdoInt32 value)
{
	m_currentBand = value - 1;
}

FdoDataValue* FdoRfpRaster::GetNullPixelValue ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	return band->GetNullPixelValue();
}

FdoRfpRaster::~FdoRfpRaster()
{
}

void FdoRfpRaster::Dispose()
{
	delete this;
}


///<summary>Returns true if the FdoIRaster represents a null value.</summary>
/// <returns>Returns true if the FdoIRaster represents a null value.</returns> 
bool FdoRfpRaster::IsNull ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	return band->IsNull();
}

///<summary>Sets the FdoIRaster to a null value.</summary>
/// <returns>Returns nothing</returns> 
void FdoRfpRaster::SetNull ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	band->SetNull();
}

/// Get the minimum bounding box around the image.
/// The bounds are transformed to the active spatial context.
FdoByteArray* FdoRfpRaster::GetBounds ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);

	FdoRfpRect* rect = band->GetBounds();
	FdoPtr<FdoByteArray> bounds = FdoRfpUtil::CreateGeometryAgfFromRect(*rect);

	return FDO_SAFE_ADDREF(bounds.p);
}

/// Set the minimum bounding box around the image.
/// For georeferenced images, the coordinate system is specified by the
/// spatial context in force when the object is created.  The act of
/// setting this property georeferences the image.  In order to provide
/// for non-georeferenced images, this property is Nullable (doesn't
/// need to be set).
void FdoRfpRaster::SetBounds (FdoByteArray* bounds)
{
    if (bounds == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
    FdoRfpRect rect = FdoRfpUtil::CreateRectFromGeometryAgf(bounds);

    band->SetBounds(&rect);
}

/// <summary>Gets the data model in use by this raster property.</summary>
/// <returns>Returns the current data model.</returns> 
FdoRasterDataModel* FdoRfpRaster::GetDataModel ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	FdoPtr<FdoRasterDataModel> dataModel = band->GetDataModel();
	return FDO_SAFE_ADDREF(dataModel.p);
}

/// <summary>Sets the data model used by this raster property.
/// Allowed values are only those data models that are acceptable to the
/// SupportsDataModel capability.
/// This attribute specifies the colour/palette model, bits per pixel,
/// tiling and interleaving. For example, if the bits per pixel of the
/// data model is 8 and it is set to 24, a conversion would be performed
/// when image data is requested, to return 24 bit data.  If an update
/// command is executed to save this value, the raster data would be
/// transformed and saved.  The next time this image is retrieved
/// by the client the bits per pixel value would indicate 24.</summary>
/// <param name="datamodel">The datamodel to be used.</param>
void FdoRfpRaster::SetDataModel (FdoRasterDataModel* datamodel)
{
    FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	band->SetDataModel(datamodel);
}

/// <summary>Gets the size of image file in the horizontal
/// direction in pixels (number of columns).</summary>
/// <returns>Returns the horizontal image size in pixels
/// (number of columns).</returns> 
FdoInt32 FdoRfpRaster::GetImageXSize ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	return band->GetImageXSize();
}

/// <summary>Sets the size of image file in the horizontal
/// direction in pixels (number of columns).
/// By setting the X and Y image size properties prior to getting
/// a reader for the data, the client application can control the
/// density of pixels fetched by the provider, which may reduce
/// the amount of data shipped by the reader and reduce processing
/// time significantly.  For example, a certain query in the
/// coordinate system of the raster object class returns a
/// FdoIRaster object with image size 12091 by 8043.  But the
/// image only needs to be displayed in an area 1167 by 776 on
/// the users monitor (note that the aspect ratios are the same
/// because the query is constructed based on the display area).
/// By setting the image size to these lower values the FDO raster
/// subsystem may be able to use a previously subsampled image
/// where the resolution has been reduced by a factor of 4.
/// So instead of transferring 97MB, it can transfer 6MB (a reduction
/// by 4 in both dimensions reduces the amount of data by a factor of 16)
/// and subsample the image again to the desired resolution in a
/// more timely manner (less data to process means less total time),
/// with no appreciable difference in display quality to the user.</summary>
/// <param name="size">The desired horizontal image size in pixels
/// (number of columns).</param> 
void FdoRfpRaster::SetImageXSize (FdoInt32 size)
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);    
	band->SetImageXSize(size);
}

/// <summary>Gets the size of image file in the vertical
/// direction in pixels (number of rows).</summary>
/// <returns>Returns the vertical image size in pixels
/// (number of rows).</returns> 
FdoInt32 FdoRfpRaster::GetImageYSize ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	return band->GetImageYSize();
}

/// <summary>Sets the size of image file in the vertical
/// direction in pixels (number of rows). See SetImageXSize.</summary>
/// <param name="size">The desired vertical image size in pixels
/// (number of rows).</param> 
void FdoRfpRaster::SetImageYSize (FdoInt32 size)
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	band->SetImageYSize(size);
}

/// <summary>Get a dictionary style interface to the raster properties.
/// Through this interface the non-standard properties of a raster
/// image can be determined and their values can be retrieved and set.
/// Data values are exchanged through the FdoDataValue object to
/// accommodate various data types, boolean, integer, double, string,
/// etc.</summary>
/// <returns>The dictionary of auxilliary raster properties.</returns> 
FdoIRasterPropertyDictionary* FdoRfpRaster::GetAuxiliaryProperties ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	FdoPtr<FdoIRasterPropertyDictionary> prop = band->GetAuxiliaryProperties();
	return FDO_SAFE_ADDREF(prop.p);
}

/// <summary>Set the source of image data.
/// Image data is shipped using a paradigm similar to BLOB I/O.
/// When inserting or updating an object with a raster property,
/// the SetStreamReader method provides a means to set a
/// FdoIStreamReader callback.
/// On execution of an insert or update command, requests are made on
/// the reader for the image data (which is supplied by the FDO client
/// application).
/// The format of the image data expected is determined by the
/// DataModel property.  The data type, bit depth, tiling and
/// organization specify the meaning of the image data.
/// By setting the DataModel property prior to setting a
/// reader for the data, the FDO client application can control the
/// way data is understood by, the FDO raster subsystem.
/// </summary>
void FdoRfpRaster::SetStreamReader (FdoIStreamReader* reader)
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	band->SetStreamReader(reader);
}

/// <summary>Get the source of image data.
/// Image data is shipped using a paradigm similar to BLOB I/O.
/// When fetching a raster image the GetStreamReader() method provides
/// a FdoIStreamReader from which the client may request the image data.
/// The format of the image data expected is determined by the
/// DataModel property.  The data type, bit depth, tiling and
/// organization specify the meaning of the image data.
/// By setting the DataModel property prior to getting a
/// reader for the data, the FDO client application can control the
/// way data is retrieved from the FDO raster subsystem.
/// When a tiled image is fetched, the image data will be tiled by default,
/// unless a spatial query is used that covers a portion of a tile,
/// in which case the image is converted to monolithic form prior to
/// returning to the client.  The client may force a monolithic
/// image in the former case by setting the DataModel to monolithic
/// prior to getting the stream reader.  Or conversely it can request
/// a re-tiling of the remaining sub-image by setting the DataModel
/// property to tiled (in this case the tile origin is at the new upper
/// left corner of the sub-image).
/// Note that altering the DataModel may have significant performance issues.
/// </summary>
FdoIStreamReader* FdoRfpRaster::GetStreamReader ()
{
	FdoPtr<FdoRfpBandRaster> band = m_bandRasters->GetItem(m_currentBand);
	FdoPtr<FdoIStreamReader> streamReader = band->GetStreamReader();
	return FDO_SAFE_ADDREF(streamReader.p);
}

/// \brief
/// Gets the Vertical Unit of Measure corresponding to the Raster Band.
/// The string value returned will be a considered lookup value that
/// will be used to identify the properties of the Unit of Measure using
/// the FDO FdoIGetMeasureUnits command.
///
/// \return
/// Returns the Unit of Measure type of data associated to the Raster Band.
FdoString* FdoRfpRaster::GetVerticalUnits ()
{
    return L"";
}

/// \brief
/// Sets the Vertical Unit of Measure corresponding to the Raster Band.
///
/// \param units
/// The Unit of Measure type for the current band.
/// The units string value corresponds to a Unit of Measure supported
/// through the FdoIGetMeasureUnits command.
void FdoRfpRaster::SetVerticalUnits (FdoString* units)
{
    throw FdoException::Create (NlsMsgGet(GRFP_5_COMMAND_TIMEOUT_NOT_SUPPORTED, "FdoIRaster::SetVerticalUnits is not supported."));
}

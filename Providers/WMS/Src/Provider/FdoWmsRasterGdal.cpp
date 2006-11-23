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
#include "stdafx.h"
#include <gdal.h>
#include <gdal_priv.h>
#include <FdoWmsRasterGdal.h>
#include <FdoWmsBandRasterGdal.h>
#include <FdoWmsRect.h>
#include <FdoWmsImage.h>
#include <Geometry/EnvelopeImpl.h>


FdoWmsRasterGdal::FdoWmsRasterGdal(GDALDataset* gdalDataSet, FdoWmsRect* bounds)
				: m_currentBand(0), m_gdalDataSet(gdalDataSet), m_bounds(bounds),
                m_rasterBuffer(NULL), m_rasterBufferSize(0), m_rasterBufferCapacity(0)
{
}

FdoWmsRasterGdal::FdoWmsRasterGdal(FdoIoStream* rasterStream, FdoWmsRect* bounds)
				: m_currentBand(0), m_bounds(bounds), 
				m_rasterStream(rasterStream), m_gdalDataSet(NULL),
				m_rasterBuffer(NULL), m_rasterBufferSize(0), m_rasterBufferCapacity(0)
{
    FDO_SAFE_ADDREF(rasterStream);
}

GDALDataset* FdoWmsRasterGdal::_getDataset() {
    if (m_gdalDataSet == NULL) {
        m_rasterBufferCapacity = 50 * 1024; // 50K might be a typical size of images from WMS servers
        m_rasterBufferSize = 0;
        m_rasterBuffer = new FdoByte[m_rasterBufferCapacity];
	    do {
	        FdoSize cntRead = m_rasterStream->Read (m_rasterBuffer + m_rasterBufferSize, m_rasterBufferCapacity - m_rasterBufferSize);
            if (cntRead == 0)
                break;
            m_rasterBufferSize += (FdoInt32)cntRead;
            // if the buffer is not large enough to hold the raster data, we need to enlarge the buffer
            if (m_rasterBufferSize == m_rasterBufferCapacity) {
                FdoByte* newBuffer = new FdoByte[m_rasterBufferCapacity * 2];
                memcpy(newBuffer, m_rasterBuffer, m_rasterBufferCapacity);
                delete[] m_rasterBuffer;
                m_rasterBuffer = newBuffer;
                m_rasterBufferCapacity *= 2;
            }

	    } while (true);

		// we should reset the stream so later calls to "GetRaster" work fine.
		m_rasterStream->Reset ();
        // now we can release the raster stream since everything has been dumped into the buffer
        m_rasterStream = NULL;
        // now create the virtual memory file from the buffer
        // NOTE: the file name for the virtual file must start with "/vsimem/" so that it could be recogized by 
        // GDAL as a virtual mem file
        char vfileName[64];
        sprintf(vfileName, "/vsimem/%d", (FdoSize)this);
        FILE *fp = VSIFileFromMemBuffer(vfileName, m_rasterBuffer, m_rasterBufferSize, FALSE );
        if( fp == NULL )
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_57_UNEXPECTEDERROR, "Unexpected error encountered."));
        VSIFCloseL( fp );
	    m_gdalDataSet = (GDALDataset*)GDALOpen(vfileName, GA_ReadOnly);
		if (!m_gdalDataSet)
			throw FdoException::Create(FdoException::NLSGetMessage(FDOWMS_12006_NO_DATA_RETURNED, "The WMS Server returned no data from the GetMap request."));
    }
    return m_gdalDataSet;
}


FdoWmsRasterGdal::~FdoWmsRasterGdal()
{
    if (m_gdalDataSet != NULL)
	    delete m_gdalDataSet;

    if (m_rasterBuffer != NULL)
        delete[] m_rasterBuffer;
}

void FdoWmsRasterGdal::Dispose()
{
	delete this;
}


FdoInt32 FdoWmsRasterGdal::GetNumberOfBands ()
{
    _getRasterBands();
	return m_bandSize;
}

void FdoWmsRasterGdal::SetNumberOfBands (FdoInt32 value)
{
	m_bandSize = value;
}

/*
	for the clients use the band number starting with 1, and internally it start from 0.
	so there is a little difference. we have to change the number between 
*/
FdoInt32 FdoWmsRasterGdal::GetCurrentBand ()
{
	return m_currentBand + 1;
}

void FdoWmsRasterGdal::SetCurrentBand (FdoInt32 value)
{
	m_currentBand = value - 1;
}

FdoDataValue* FdoWmsRasterGdal::GetNullPixelValue ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->GetNullPixelValue();
}

bool FdoWmsRasterGdal::IsNull ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->IsNull();
}

///<summary>Sets the FdoIRaster to a null value.</summary>
/// <returns>Returns nothing</returns> 
void FdoWmsRasterGdal::SetNull ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	bandRaster->SetNull();
}

/// Get the minimum bounding box around the image.
/// The bounds are transformed to the active spatial context.
FdoByteArray* FdoWmsRasterGdal::GetBounds ()
{
	if (!m_bounds)
		return NULL;
	// By its current implemention, all binds share the same bounds that was input when constructing
	// this object, so it's unnecessary to get the bounds from each band. In other words, all bands
	// have been constructed unnecessarily in order to get the bounds of a specific bind.
	FdoPtr<FdoFgfGeometryFactory> geomfactory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoEnvelopeImpl> envelope = FdoEnvelopeImpl::Create(m_bounds->m_minX, m_bounds->m_minY, m_bounds->m_maxX, m_bounds->m_maxY);
	FdoPtr<FdoIGeometry> iGeometery = geomfactory->CreateGeometry(envelope);
	FdoPtr<FdoByteArray> bounds = geomfactory->GetFgf(iGeometery);
	return FDO_SAFE_ADDREF(bounds.p);
}

/// Set the minimum bounding box around the image.
/// For georeferenced images, the coordinate system is specified by the
/// spatial context in force when the object is created.  The act of
/// setting this property georeferences the image.  In order to provide
/// for non-georeferenced images, this property is Nullable (doesn't
/// need to be set).
void FdoWmsRasterGdal::SetBounds (FdoByteArray* bounds)
{
    FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	bandRaster->SetBounds(bounds);
}

/// <summary>Gets the data model in use by this raster property.</summary>
/// <returns>Returns the current data model.</returns> 
FdoRasterDataModel* FdoWmsRasterGdal::GetDataModel ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->GetDataModel();
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
void FdoWmsRasterGdal::SetDataModel (FdoRasterDataModel* dataModel)
{
    FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	bandRaster->SetDataModel(dataModel);
}

/// <summary>Gets the size of image file in the horizontal
/// direction in pixels (number of columns).</summary>
/// <returns>Returns the horizontal image size in pixels
/// (number of columns).</returns> 
FdoInt32 FdoWmsRasterGdal::GetImageXSize ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->GetImageXSize();
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
void FdoWmsRasterGdal::SetImageXSize (FdoInt32 size)
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	bandRaster->SetImageXSize(size);
}

/// <summary>Gets the size of image file in the vertical
/// direction in pixels (number of rows).</summary>
/// <returns>Returns the vertical image size in pixels
/// (number of rows).</returns> 
FdoInt32 FdoWmsRasterGdal::GetImageYSize ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->GetImageYSize();
}

/// <summary>Sets the size of image file in the vertical
/// direction in pixels (number of rows). See SetImageXSize.</summary>
/// <param name="size">The desired vertical image size in pixels
/// (number of rows).</param> 
void FdoWmsRasterGdal::SetImageYSize (FdoInt32 size)
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	bandRaster->SetImageYSize(size);
}

/// <summary>Get a dictionary style interface to the raster properties.
/// Through this interface the non-standard properties of a raster
/// image can be determined and their values can be retrieved and set.
/// Data values are exchanged through the FdoDataValue object to
/// accommodate various data types, boolean, integer, double, string,
/// etc.</summary>
/// <returns>The dictionary of auxilliary raster properties.</returns> 
FdoIRasterPropertyDictionary* FdoWmsRasterGdal::GetAuxiliaryProperties ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->GetAuxiliaryProperties();
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
void FdoWmsRasterGdal::SetStreamReader (FdoIStreamReader* reader)
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	bandRaster->SetStreamReader(reader);
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
FdoIStreamReader* FdoWmsRasterGdal::GetStreamReader ()
{
	FdoWmsBandRasterGdalP bandRaster = _getRasterBands()->GetItem(m_currentBand);
	return bandRaster->GetStreamReader();
}

/// \brief
/// Gets the Vertical Unit of Measure corresponding to the Raster Band.
/// The string value returned will be a considered lookup value that
/// will be used to identify the properties of the Unit of Measure using
/// the FDO FdoIGetMeasureUnits command.
///
/// \return
/// Returns the Unit of Measure type of data associated to the Raster Band.
///
FdoString* FdoWmsRasterGdal::GetVerticalUnits ()
{
    return m_verticalUnits;
}

/// \brief
/// Sets the Vertical Unit of Measure corresponding to the Raster Band.
///
/// \param units
/// The Unit of Measure type for the current band.
/// The units string value corresponds to a Unit of Measure supported
/// through the FdoIGetMeasureUnits command.
void FdoWmsRasterGdal::SetVerticalUnits (FdoString* units)
{
    throw FdoCommandException::Create (
        NlsMsgGet(FDOWMS_OPERATION_NOT_SUPPORTED, "FDO Operation '%1$ls' is not supported.", L"FdoIRaster::SetVerticalUnits"));
}

// --------------------------------------------------
// Internally used helper functions
// --------------------------------------------------
FdoWmsBandRasterGdalCollection* FdoWmsRasterGdal::_getRasterBands(void)
{
    if (m_bandRasters == NULL) {
        m_bandRasters = FdoWmsBandRasterGdalCollection::Create();

	    int i;
	    // if there are R\G\B\A bands, merge them into a single RGBA data model
	    FdoWmsImagesP images = FdoWmsImageCollection::Create();
	    bool hasRGBA = false;
	    FdoPtr<FdoWmsImage> rgbaImage;
	    std::vector<GDALRasterBand*> rgbaBands;
	    for (i=0; i<_getDataset()->GetRasterCount(); i++)
	    {
		    GDALRasterBand* rasterBand = _getDataset()->GetRasterBand(i+1);
		    switch (rasterBand->GetColorInterpretation())
		    {
		    case GCI_RedBand:
		    case GCI_GreenBand:
		    case GCI_BlueBand:
		    case GCI_AlphaBand:
			    {
				    if (rasterBand->GetRasterDataType() != GDT_Byte)
					    throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));

				    if (hasRGBA == false) // first time to meet a R G B or A band, create the FdoWmsImage
					    hasRGBA = true;
    				
				    rgbaBands.push_back(rasterBand);
			    }
			    break;
		    case GCI_Undefined:
		    case GCI_PaletteIndex:
		    case GCI_GrayIndex:	
			    {
				    std::vector<GDALRasterBand*> bands;
				    bands.push_back(rasterBand);
				    FdoPtr<FdoWmsImage> bandImage = FdoWmsImage::Create(bands);
				    images->Add(bandImage);
			    }
			    break;
		    }
	    }

	    if (hasRGBA) // first time to meet a R G B or A band, create the FdoWmsImage
	    {
		    rgbaImage = FdoWmsImage::Create(rgbaBands);	
		    images->Add(rgbaImage);
	    }

	    SetNumberOfBands(images->GetCount());

	    for (i=0; i < images->GetCount(); i++)
	    {
		    FdoPtr<FdoWmsImage> band = images->GetItem(i);

		    // verify that the sub band number must be 1,3 or 4.
		    if (band->GetBandSize() != 1 && band->GetBandSize() != 3 && band->GetBandSize() != 4)
			    throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));

		    FdoWmsBandRasterGdalP bandRaster = new FdoWmsBandRasterGdal(band, *m_bounds);
		    m_bandRasters->Add(bandRaster);
	    }
    }

    return m_bandRasters.p;
}

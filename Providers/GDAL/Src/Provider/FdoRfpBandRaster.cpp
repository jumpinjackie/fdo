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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpBandRaster.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include <math.h>

#include "FDORFP.h"
#include "FdoRfpBandRaster.h"
#include "FdoRfpRasterCapabilities.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpStreamReaderByTile.h"
#include "FdoRfpRasterPropertyDictionary.h"
#include "FdoRfpImage.h"
#include "FdoRfpEnvelope.h"
#include "FdoRfpRasterUtil.h"
#include "FdoRfpGlobals.h"
#include "FdoRfpKeyColor.h"
#include <gdal.h>

FdoRfpBandRaster::FdoRfpBandRaster(const FdoPtr<FdoRfpGeoBandRaster>& geoBandRaster, 
                                   FdoRfpRect* clippingBounds) : 
	m_geoBandRaster(geoBandRaster), m_clippingBounds(NULL),
        m_bNull(false), m_imageXSize(NULL), m_imageYSize(NULL),
        m_resolutionX(NULL), m_resolutionY(NULL), m_bounds(NULL)
{
    _computeOriginalBounds();
    _recomputeImageSize();

    if (clippingBounds != NULL)
		SetBounds( clippingBounds );
}

FdoRfpBandRaster::~FdoRfpBandRaster()
{
    SetNull();

    if( m_imageXSize )
    {
        delete m_imageXSize;
        m_imageXSize = NULL;
    }

    if( m_imageYSize )
    {
        delete m_imageYSize;
        m_imageYSize = NULL;
    }

    if( m_resolutionX )
    {
        delete m_resolutionX;
        m_resolutionX = NULL;
    }

    if( m_resolutionY )
    {
        delete m_resolutionY;
        m_resolutionY = NULL;
    }

    if( m_bounds )
    {
        delete m_bounds;
        m_bounds = NULL;
    }
}

void FdoRfpBandRaster::Dispose()
{
    delete this;
}

// Validate the raster is not null before any data retrieval operations
void FdoRfpBandRaster::_validate()
{
    if (m_bNull)
        throw FdoCommandException::Create(NlsMsgGet(GRFP_64_RASTER_NULL, "Operation failed because the raster is null."));
}

///<summary>Returns true if the FdoIRaster represents a null value.</summary>
/// <returns>Returns true if the FdoIRaster represents a null value.</returns> 
bool FdoRfpBandRaster::IsNull ()
{
    return m_bNull;
}

///<summary>Sets the FdoIRaster to a null value.</summary>
/// <returns>Returns nothing</returns> 
void FdoRfpBandRaster::SetNull ()
{
    m_bNull = true;
    m_geoBandRaster = NULL;

    if (m_clippingBounds != NULL)
    {
        delete m_clippingBounds;
        m_clippingBounds = NULL;
    }
}

/// Get the minimum bounding box around the image.
/// The bounds are transformed to the active spatial context.
FdoRfpRect* FdoRfpBandRaster::GetBounds ()
{
    _validate();

    if( m_clippingBounds != NULL )
        return m_clippingBounds;

    else if( m_bounds == NULL )
    {
        FdoRfpRect rect = _getRequestBounds();
        m_bounds = new FdoRfpRect(rect.m_minX, rect.m_minY, rect.m_maxX, rect.m_maxY);
    }

    return m_bounds;
}

/// Set the minimum bounding box around the image.
/// For georeferenced images, the coordinate system is specified by the
/// spatial context in force when the object is created.  The act of
/// setting this property georeferences the image.  In order to provide
/// for non-georeferenced images, this property is Nullable (doesn't
/// need to be set).
void FdoRfpBandRaster::SetBounds (FdoRfpRect* bounds)
{
    if (bounds == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    _validate();

    if( m_clippingBounds )
        delete m_clippingBounds;

    m_clippingBounds = 
        new FdoRfpRect( *bounds );

    // reduce clipping bounds to available data. 
    *m_clippingBounds = m_clippingBounds->Intersect( *m_bounds );
    
    // Clip to nearest pixel edge. 
    double baseResolutionX = m_geoBandRaster->GetResolutionX();
    double baseResolutionY = m_geoBandRaster->GetResolutionY();

    m_clippingBounds->m_minX = 
        floor((m_clippingBounds->m_minX - m_bounds->m_minX) / baseResolutionX + 0.0001 ) * baseResolutionX
        + m_bounds->m_minX;
    m_clippingBounds->m_minY = 
        floor((m_clippingBounds->m_minY - m_bounds->m_minY) / baseResolutionY + 0.0001 ) * baseResolutionY
        + m_bounds->m_minY;
    m_clippingBounds->m_maxX = 
        ceil( (m_clippingBounds->m_maxX - m_bounds->m_minX) / baseResolutionX - 0.0001 ) * baseResolutionX
        + m_bounds->m_minX;
    m_clippingBounds->m_maxY = 
        ceil( (m_clippingBounds->m_maxY - m_bounds->m_minY) / baseResolutionY - 0.0001 ) * baseResolutionY
        + m_bounds->m_minY;

    // Force the image size to update instead of resolution.
    _recomputeImageSize();

    // This fiddle with resolution based on selected image size to make things "exact".
    *m_resolutionX = m_clippingBounds->GetWidth() / GetImageXSize();
    *m_resolutionY = m_clippingBounds->GetHeight() / GetImageYSize();
}

/// <summary>Gets the data model in use by this raster property.</summary>
/// <returns>Returns the current data model.</returns> 
FdoRasterDataModel* FdoRfpBandRaster::GetDataModel ()
{
    _validate();

    if( m_dataModel != NULL )
        return FDO_SAFE_ADDREF(m_dataModel.p);

    FdoPtr<FdoRasterDataModel> dataModel = FdoRasterDataModel::Create();
    FdoPtr<FdoRfpImage> image =m_geoBandRaster->GetImage();

    if( image->m_components == 4 )
        dataModel->SetDataModelType(FdoRasterDataModelType_RGBA);
    else if( image->m_components == 3 )
        dataModel->SetDataModelType(FdoRasterDataModelType_RGB);
    else if( GDALGetRasterColorInterpretation( GDALGetRasterBand(image->GetDS(), image->m_bandList[0]) ) 
             == GCI_PaletteIndex )
        dataModel->SetDataModelType(FdoRasterDataModelType_Palette);
    else
        dataModel->SetDataModelType(FdoRasterDataModelType_Gray);

    if( image->m_gdalDataType == GDT_Float32 
        || image->m_gdalDataType == GDT_Float64 )
        dataModel->SetDataType( FdoRasterDataType_Float );
    else if( image->m_gdalDataType == GDT_Int16 
             || image->m_gdalDataType == GDT_Int32 )
        dataModel->SetDataType( FdoRasterDataType_Integer );
    else
        dataModel->SetDataType( FdoRasterDataType_UnsignedInteger );

    dataModel->SetBitsPerPixel( image->m_bytesPerPixel * 8 );

    // set organization to pixel
    dataModel->SetOrganization( FdoRasterDataOrganization_Pixel );

    // Typical "web" use produces modest screen sized images.  For these
    // we might as well treat them as one big tile, as this reduces to one
    // big efficient read in GDAL.  A single RasterIO() call.  For big requested
    // images we treat one row as a tile so we never use too much memory.

    // TODO: We really should be using the tilesize provided by the application.
    if( GetImageXSize() * (double) GetImageYSize() > 4000000 )
    {
        dataModel->SetTileSizeX(GetImageXSize());
        dataModel->SetTileSizeY(1);
    }
    else
    {
        dataModel->SetTileSizeX( GetImageXSize() );
        dataModel->SetTileSizeY( GetImageYSize() );
    }

    m_dataModel = dataModel;

    image->ReleaseDS();

    return FDO_SAFE_ADDREF(m_dataModel.p);
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
void FdoRfpBandRaster::SetDataModel (FdoRasterDataModel* datamodel)
{
    if (datamodel == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    _validate();
    FdoRfpRasterCapabilitiesP rasterCap = new FdoRfpRasterCapabilities();
    if (!rasterCap->SupportsDataModel(datamodel))
        throw FdoCommandException::Create(NlsMsgGet(GRFP_65_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));
    m_dataModel = FDO_SAFE_ADDREF(datamodel);
}

/// <summary>Gets the size of image file in the horizontal
/// direction in pixels (number of columns).</summary>
/// <returns>Returns the horizontal image size in pixels
/// (number of columns).</returns> 
FdoInt32 FdoRfpBandRaster::GetImageXSize ()
{
    _validate();
    if (m_imageXSize == NULL)
        _recomputeImageSize();

    return *m_imageXSize;
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
								
void FdoRfpBandRaster::SetImageXSize (FdoInt32 size)
{
    if (size <= 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    _validate();
    
    *m_imageXSize = size;
    *m_resolutionX = GetBounds()->GetWidth() / size;

    if( m_dataModel != NULL )
    {
        m_dataModel->SetTileSizeX( GetImageXSize() );
        m_dataModel->SetTileSizeY( GetImageYSize() );
    }
}

/// <summary>Gets the size of image file in the vertical
/// direction in pixels (number of rows).</summary>
/// <returns>Returns the vertical image size in pixels
/// (number of rows).</returns> 
FdoInt32 FdoRfpBandRaster::GetImageYSize ()
{
    _validate();
    if (m_imageYSize == NULL)
        _recomputeImageSize();

    return *m_imageYSize;
}

/// <summary>Sets the size of image file in the vertical
/// direction in pixels (number of rows). See SetImageXSize.</summary>
/// <param name="size">The desired vertical image size in pixels
/// (number of rows).</param> 

void FdoRfpBandRaster::SetImageYSize (FdoInt32 size)
{
    if (size <= 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    _validate();

    *m_imageYSize = size;
    *m_resolutionY = GetBounds()->GetHeight() / size;

    if( m_dataModel != NULL )
    {
        m_dataModel->SetTileSizeX( GetImageXSize() );
        m_dataModel->SetTileSizeY( GetImageYSize() );
    }
}

/// <summary>Get a dictionary style interface to the raster properties.
/// Through this interface the non-standard properties of a raster
/// image can be determined and their values can be retrieved and set.
/// Data values are exchanged through the FdoDataValue object to
/// accommodate various data types, boolean, integer, double, string,
/// etc.</summary>
/// <returns>The dictionary of auxilliary raster properties.</returns> 
FdoIRasterPropertyDictionary* FdoRfpBandRaster::GetAuxiliaryProperties ()
{
    _validate();

    // we must get stream reader first to refresh raster properties
    // because user may change some raster parameters
    FdoPtr<FdoIStreamReader> reader = GetStreamReader();

    return new FdoRfpRasterPropertyDictionay(this);
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
void FdoRfpBandRaster::SetStreamReader (FdoIStreamReader* reader)
{
    _validate();

    throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, 
                                                 "'%1$ls' not supported.", L"SetStreamReader"));
}

/// <summary> Gets the pixel value that represents a NULL value in the available raster stream. </summary>
/// <returns> Returns the FdoDataType value that can be used to identify the Null value. </returns> 
/// <remarks> The Null Pixel Value is Data Model dependent. The value of the Null pixel
/// will change depending on which data model is set. If the client application changes 
/// the Data Model using SetDataModel(FdoRasterDataModel* datamodel) a subsequent 
/// call to GetNullPixelValue will need to be made in order to determine the valid Null 
/// value for the current data model.</remarks>
FdoDataValue* FdoRfpBandRaster::GetNullPixelValue ()
{	
    FdoPtr<FdoDataValue> val;
    FdoRasterDataModel* dataModel = GetDataModel ();
    FdoPtr<FdoRfpImage> image = m_geoBandRaster->GetImage();

    int bGotNoData;
    double dfNoData;
    
    dfNoData = GDALGetRasterNoDataValue( GDALGetRasterBand( image->GetDS(), image->m_bandList[0]), &bGotNoData );

    if( image->m_components == 1 && bGotNoData )
    {
        if( image->m_gdalDataType == GDT_Byte )
            val = FdoDataValue::Create((FdoInt8) dfNoData);
        else if( image->m_gdalDataType == GDT_Int16 )
            val = FdoDataValue::Create((FdoInt16) dfNoData);
        else if( image->m_gdalDataType == GDT_Int32 )
            val = FdoDataValue::Create((FdoInt32) dfNoData);
        else if( image->m_gdalDataType == GDT_Float32 )
            val = FdoDataValue::Create((float) dfNoData);
        else // TODO: How important is it to return the right type?  What about unsigned?
            val = FdoDoubleValue::Create(dfNoData);
    }

    // TODO: Add support for NODATA_VALUES metadata, like the png&mrsid drivers return.

    return FDO_SAFE_ADDREF(val.p);
}

int FdoRfpBandRaster::_getConversionOptions()
{
    // find out which conversions(retiling, scaling, data model changing) are needed
    int options = CO_NOCONVERSION;

    // Whether scaling is needed
    if ((m_imageXSize != NULL && *m_imageXSize != GetImageXSize()) ||
        (m_imageYSize != NULL && *m_imageYSize != GetImageYSize()))
        options |= CO_SCALING;

    if (m_dataModel != NULL)
    {
        FdoPtr<FdoRasterDataModel> dataModel = GetDataModel();

        // TODO: This logic does not really work because 
        // GetDataModel() will return m_dataModel if it is set. 
        // Perhaps we could reset the internal pointer so GetDataModel()
        // would return a clean copy of the data model.  But we would 
        // need to put the possibly modified data model back in place
        // when done. 

        // Whether retiling is needed
        if (m_dataModel->GetTileSizeX() != dataModel->GetTileSizeX() ||
            m_dataModel->GetTileSizeY() != dataModel->GetTileSizeY())
            options |= CO_RETILING;

        // Whether data model changing is needed
        if (m_dataModel->GetDataModelType() != dataModel->GetDataModelType() ||
            m_dataModel->GetBitsPerPixel() != dataModel->GetBitsPerPixel())
            options |= CO_DATAMODELCHANGING;

    }
    return options;
}

void FdoRfpBandRaster::_getResolution()
{
    if (m_resolutionX == NULL)
    {
        // use the first raster's resolution
        m_resolutionX = new double(m_geoBandRaster->GetResolutionX());
        m_resolutionY = new double(m_geoBandRaster->GetResolutionY());
    }
}

// accessor for m_resulotionX/Y
double FdoRfpBandRaster::_getResolutionX()
{
    if (m_resolutionX == NULL)
        _getResolution();
    return *m_resolutionX;
}
double FdoRfpBandRaster::_getResolutionY()
{
    if (m_resolutionY == NULL)
        _getResolution();
    return *m_resolutionY;
}

// Compute the request window in pixels and lines, and then reset the clipping
// rectangle to ensure it falls exactly on pixel boundaries.

void FdoRfpBandRaster::_computePixelWindow(
    const FdoPtr<FdoRfpImage>& image,
    int &winXOff, int &winYOff, int &winXSize, int &winYSize )

{
    FdoRfpRect origBounds( *m_bounds );
    FdoRfpRect viewBounds = _getRequestBounds();
    double origResolutionX, origResolutionY;

    origResolutionX = (origBounds.m_maxX - origBounds.m_minX) / image->m_xSize;
    origResolutionY = -1 * (origBounds.m_maxY - origBounds.m_minY) / image->m_ySize;

    winXOff = (int) floor((viewBounds.m_minX - origBounds.m_minX) / origResolutionX);
    winYOff = (int) floor((viewBounds.m_maxY - origBounds.m_maxY) / origResolutionY);

    int nLRX = (int) ceil((viewBounds.m_maxX - origBounds.m_minX) / origResolutionX);
    int nLRY = (int) ceil((viewBounds.m_minY - origBounds.m_maxY) / origResolutionY);

    winXSize = nLRX - winXOff;
    winYSize = nLRY - winYOff;
}

FdoRfpRect FdoRfpBandRaster::_getRequestBounds()
{
    FdoRfpRect rv( *m_bounds );
    if (m_clippingBounds != NULL)
        rv = rv.Intersect(*m_clippingBounds);
    return rv;
}

void FdoRfpBandRaster::_recomputeImageSize()

{
    if( m_imageXSize == NULL )
    {
        m_imageXSize = new FdoInt32;
        m_imageYSize = new FdoInt32;
    }

    *m_imageXSize = (FdoInt32)floor((_getRequestBounds().GetWidth() / _getResolutionX()) + 0.5);
    if( *m_imageXSize < 1 )
        *m_imageXSize = 1;

    *m_imageYSize = (FdoInt32)floor((_getRequestBounds().GetHeight() / _getResolutionY()) + 0.5);
    if( *m_imageYSize < 1 )
        *m_imageYSize = 1;


    if( m_dataModel != NULL )
    {
        m_dataModel->SetTileSizeX( GetImageXSize() );
        m_dataModel->SetTileSizeY( GetImageYSize() );
    }
}

void FdoRfpBandRaster::_computeOriginalBounds()
{
    m_bounds = new FdoRfpRect( m_geoBandRaster->GetBounds() );
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

// Get the raster data
// Parameter options indicates that whether a raster conversion is needed. Three types of conversion
// are possible: retiling, scaling, and data model changing. Any combination of the three
// types are possible.
FdoIStreamReader* FdoRfpBandRaster::GetStreamReader()
{
    _validate();

    int options = _getConversionOptions();

    FdoPtr<FdoRfpImage> image = m_geoBandRaster->GetImage();

    FdoPtr<FdoIStreamReader> streamReader;

    // make clear our "target"
    FdoPtr<FdoRasterDataModel> targetDataModel = GetDataModel();
    FdoRfpRect originalBounds( *m_bounds );
    FdoRfpRect requestBounds = _getRequestBounds();

    // if the user want to return raster data tile by tile and the tile size happens to
    // be same as the original tile size of the image and also if the user want the whole
    // image, then we could directly use StreamReaderByTile which will return the orignal
    // image data tile by tile
    int winXOff, winYOff, winXSize, winYSize;
    int tileXSize, tileYSize;

    _computePixelWindow( image, winXOff, winYOff, winXSize, winYSize );

    streamReader = new FdoRfpStreamReaderGdalByTile(image, 
                                                    targetDataModel,
                                                    winXOff, winYOff, winXSize, winYSize, 
                                                    GetImageXSize(), GetImageYSize());

    return FDO_SAFE_ADDREF(streamReader.p);
}

FdoRfpImage *FdoRfpBandRaster::GetImage()

{
    return m_geoBandRaster->GetImage();
}

FdoRfpBandRasterCollection* FdoRfpBandRasterCollection::Create()
{
    return new FdoRfpBandRasterCollection();
}

void FdoRfpBandRasterCollection::Dispose()
{
    delete this;
}


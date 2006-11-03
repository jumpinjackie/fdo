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
#include <FdoWmsImage.h>
#include <gdal.h>
#include <gdal_priv.h>


FdoWmsImage::FdoWmsImage() : m_palette(NULL), m_imageXSize(0), m_imageYSize(0), m_numberOfPaletteEntries(0), m_imageXSizeChanged(0), m_imageYSizeChanged(0)
{
}

FdoWmsImage::FdoWmsImage(std::vector<GDALRasterBand*>& rasterBands) : m_bands(rasterBands), m_palette(NULL), m_buffer(NULL)
{
	m_imageXSizeChanged = 0;
	m_imageYSizeChanged = 0;

	_getImageSize();	
	_getDataModel();
	_getNullPixelValue();
}

FdoWmsImage::~FdoWmsImage()
{
	// just set raster band data to be NULL.
	// gdal raster band should not be released here. Actually it only records 
	// a reference to the real raster band in the gdal dataset. The dataset is
	// responsible for releasing data of raster band. 
	int i;
	for (i=0; i < GetBandSize(); i++)
		m_bands[i] = NULL;

	delete[] m_palette;
	m_palette = NULL;

	_destroyBuffer();
}

void FdoWmsImage::AddBand(GDALRasterBand* band)
{
	m_bands.push_back(band); 
}
FdoInt32 FdoWmsImage::GetBandSize()
{
	return (FdoInt32)m_bands.size();
}

FdoRasterDataModel* FdoWmsImage::GetDataModel()
{
	return FDO_SAFE_ADDREF(m_dataModel.p);
}

void FdoWmsImage::SetDataModel(FdoRasterDataModel* dataModel)
{
	m_dataModel = FDO_SAFE_ADDREF(dataModel);
}

FdoInt32 FdoWmsImage::GetImageXSize()
{
	return m_imageXSize;
}

FdoInt32 FdoWmsImage::GetImageYSize()
{
	return m_imageYSize;
}

void FdoWmsImage::SetImageXSize(FdoInt32 size)
{
	m_imageXSize = size;
}

void FdoWmsImage::SetImageYSize(FdoInt32 size)
{
	m_imageYSize = size;
}

FdoDataValue* FdoWmsImage::GetNullPixelValue()
{
	return FDO_SAFE_ADDREF(m_nullPixelValue.p);
}

FdoInt32 FdoWmsImage::GetNumberOfPaletteEntries()
{
	return m_numberOfPaletteEntries;
}

RgbColor* FdoWmsImage::GetPalette()
{
	return m_palette;
}

FdoInt32 FdoWmsImage::GetBytesPerRow()
{
	return m_imageXSize * m_dataModel->GetBitsPerPixel() / 8;
}

FdoByte* FdoWmsImage::GetBuffer()
{
	if (m_buffer == NULL || m_imageXSize != m_imageXSizeChanged || m_imageYSize != m_imageYSizeChanged)
		_getBuffer();

	return m_buffer;
}

FdoInt64 FdoWmsImage::GetLength()
{
	return m_imageXSize * m_imageYSize * m_dataModel->GetBitsPerPixel() / 8;
}

// -----------------------------------------
// internal helper functions
// -----------------------------------------
void FdoWmsImage::_getImageSize()
{
	m_imageXSize = 0;
	m_imageYSize = 0;
	for (int i=0; i < GetBandSize(); i++)
	{
		if (m_bands[i]->GetXSize() > m_imageXSize)
			m_imageXSize = m_bands[i]->GetXSize();
		if (m_bands[i]->GetYSize() > m_imageYSize)
			m_imageYSize = m_bands[i]->GetYSize();
	}
}

void FdoWmsImage::_getNullPixelValue()
{
	int bSuccess;
	FdoInt32 bandSize = GetBandSize();
	if (bandSize >= 3) // RGB or RGBA
	{		
		RgbColor rc;
		rc.rgba.red   = (FdoByte)m_bands[0]->GetNoDataValue(&bSuccess);
		if (!bSuccess)
			rc.rgba.red = 0;

		rc.rgba.green = (FdoByte)m_bands[1]->GetNoDataValue(&bSuccess);
		if (!bSuccess)
			rc.rgba.green = 0;

		rc.rgba.blue  = (FdoByte)m_bands[2]->GetNoDataValue(&bSuccess);
		if (!bSuccess)
			rc.rgba.blue = 0;

		if (bandSize == 4)
		{
			rc.rgba.alpha = (FdoByte)m_bands[3]->GetNoDataValue(&bSuccess);
			if (!bSuccess)
				rc.rgba.alpha = 0;
		}
		else
			rc.rgba.alpha = (FdoByte)0;

		m_nullPixelValue = FdoInt32Value::Create(rc.packed);
	}
	else if (bandSize == 1)
	{
		// determine the bits per pixel		
		double null_val = m_bands[0]->GetNoDataValue(&bSuccess);
		if (!bSuccess)
			null_val = 0;

		switch (m_bands[0]->GetRasterDataType())
		{
		case GDT_Byte:
			m_nullPixelValue = FdoByteValue::Create((FdoByte)null_val);
			break;
		case GDT_UInt16:
		case GDT_Int16:
			m_nullPixelValue = FdoInt16Value::Create((FdoInt16)null_val);
			break;
		case GDT_UInt32:
		case GDT_Int32:
			m_nullPixelValue = FdoInt32Value::Create((FdoInt32)null_val);
			break;
		case GDT_Float32:
			m_nullPixelValue = FdoSingleValue::Create((float)null_val);
			break;
		case GDT_Float64:
		case GDT_CInt16:
		case GDT_CInt32:
		case GDT_CFloat32:
		case GDT_CFloat64:
		default:
			throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));
			break;
		}
	}
}
void FdoWmsImage::_getDataModel()
{
	m_dataModel = FdoRasterDataModel::Create();

	FdoInt32 bandSize = GetBandSize();
	if (bandSize == 3) // it's RGB datamodel, only these two models will have multiple sub bands
	{
		m_dataModel->SetDataModelType(FdoRasterDataModelType_RGB);
		m_dataModel->SetBitsPerPixel(24);
	}
	else if (bandSize == 4) // RGBA
	{
		m_dataModel->SetDataModelType(FdoRasterDataModelType_RGBA);
		m_dataModel->SetBitsPerPixel(32);
	}
	else if (bandSize == 1) // there is only one sub band
	{
		GDALRasterBand* band = m_bands[0];
		GDALColorInterp colorInterp = band->GetColorInterpretation();
		switch (colorInterp)
		{
		case GCI_GrayIndex:
			m_dataModel->SetDataModelType(FdoRasterDataModelType_Gray);
			break;
		case GCI_PaletteIndex:
			{
				m_dataModel->SetDataModelType(FdoRasterDataModelType_Palette);

				GDALColorTable* palette = band->GetColorTable();				
				GDALPaletteInterp palInterp = palette->GetPaletteInterpretation();

				int i;
				const GDALColorEntry* colorEntry;
				m_numberOfPaletteEntries = palette->GetColorEntryCount();
				m_palette = new RgbColor[m_numberOfPaletteEntries];
				switch (palInterp)
				{
				case GPI_Gray:
					{
						for (i=0; i<m_numberOfPaletteEntries; i++)
						{
							colorEntry = palette->GetColorEntry(i);
							m_palette[i].rgba.red = (FdoByte)colorEntry[i].c1;
						}
					}
					break;
				case GPI_RGB:
					{
						for (i=0; i<m_numberOfPaletteEntries; i++)
						{
							colorEntry = palette->GetColorEntry(i);
							m_palette[i].rgba.red   = (FdoByte)colorEntry->c1;
							m_palette[i].rgba.green = (FdoByte)colorEntry->c2;
							m_palette[i].rgba.blue  = (FdoByte)colorEntry->c3;
							m_palette[i].rgba.alpha = (FdoByte)colorEntry->c4;
						}
					}
					break;
				default:
					throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));
					break;
				}
			}
			break;
		case GCI_Undefined:
			{
				m_dataModel->SetDataModelType(FdoRasterDataModelType_Data);

				FdoRasterDataType dataType = FdoRasterDataType_Unknown;
				if (band->GetRasterDataType() == GDT_UInt32)
					dataType = FdoRasterDataType_UnsignedInteger;
				else if (band->GetRasterDataType() == GDT_Int32)
					dataType = FdoRasterDataType_Integer;
				else if (band->GetRasterDataType() == GDT_Float32)
					dataType = FdoRasterDataType_Float;
				else 
					throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));

				m_dataModel->SetDataType(dataType);
			}
			break;
		default:
			throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));
			break;
		}	

		// determine the bits per pixel
		switch (band->GetRasterDataType())
		{
		case GDT_Byte:
			m_dataModel->SetBitsPerPixel(8);
			break;
		case GDT_UInt16:
		case GDT_Int16:
			m_dataModel->SetBitsPerPixel(16);
			break;
		case GDT_UInt32:
		case GDT_Int32:
		case GDT_Float32:
			m_dataModel->SetBitsPerPixel(32);
			break;
		case GDT_Float64:
			m_dataModel->SetBitsPerPixel(64);
			break;
		case GDT_CInt16:
		case GDT_CInt32:
		case GDT_CFloat32:
		case GDT_CFloat64:
		default:
			throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));
			break;
		}
	}

	// set organization to pixel
	m_dataModel->SetOrganization(FdoRasterDataOrganization_Pixel);
	// no tile by default
	m_dataModel->SetTileSizeX(GetImageXSize());
	m_dataModel->SetTileSizeY(GetImageYSize());
}

void FdoWmsImage::_getBuffer()
{
	_destroyBuffer();

	FdoInt64 length = GetLength();
	m_buffer = new FdoByte[(size_t)length];

	int i, j;
	FdoInt32 bandSize = GetBandSize();
	if (bandSize >= 3) // if it's R,G,B,A data model, merge them into one single RGBA stream
	{
		FdoByte* buffer_band = new FdoByte[(size_t)(length/bandSize)];
		for (i=0; i < bandSize; i++)
		{		
			m_bands[i]->RasterIO(GF_Read, 0, 0, m_bands[i]->GetXSize(), m_bands[i]->GetYSize(), buffer_band, m_imageXSize, m_imageYSize, m_bands[i]->GetRasterDataType(), 0 , 0);
			for (j=0; j < length/bandSize; j++)
				m_buffer[bandSize*j + i] = buffer_band[j];			
		}
		delete[] buffer_band; buffer_band = NULL;
	}
	else // band size = 1
	{
		m_bands[0]->RasterIO(GF_Read, 0, 0, m_bands[0]->GetXSize(), m_bands[0]->GetYSize(), m_buffer, m_imageXSize, m_imageYSize, m_bands[0]->GetRasterDataType(), 0 , 0);
	}

	m_imageXSizeChanged = m_imageXSize;
	m_imageYSizeChanged = m_imageYSize;
}

void FdoWmsImage::_destroyBuffer()
{
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = NULL;	
	}
	m_length = 0;
}

// -----------------------------------------
// collection of FdoWmsImage
// -----------------------------------------
FdoWmsImageCollection::FdoWmsImageCollection()
{
}

FdoWmsImageCollection::~FdoWmsImageCollection()
{
}

void FdoWmsImageCollection::Dispose()
{
	delete this;
}

FdoWmsImageCollection* FdoWmsImageCollection::Create()
{
	return new FdoWmsImageCollection();
}

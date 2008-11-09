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
#ifndef FdoWmsImage_H
#define FdoWmsImage_H

#include <vector>

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRasterDataModel;
class GDALRasterBand;

struct RgbColor
{
	union 
	{
		struct { FdoByte red; FdoByte green; FdoByte blue; FdoByte alpha; } rgba;
		FdoInt32 packed;
	};
};

class FdoWmsImage : public FdoIDisposable
{
//
// Data members
//
private:
	// usually there is only one band, but for RGB or RGBA data model, it would contain 3 or 4 bands: R, G, B or A.
	// it means for RGBA data model, FdoIRaster only return a single data model "RGBA" or "RGB"
	std::vector<GDALRasterBand*>	m_bands;

	// The data model of the raster
	FdoPtr<FdoRasterDataModel>		m_dataModel;

	// null pixel value
	FdoPtr<FdoDataValue>			m_nullPixelValue;

	// X size of the raster
	FdoInt32 						m_imageXSize;
	// Y size of the raster
	FdoInt32 						m_imageYSize;	

	// X size of the raster, recording the last time setting
	FdoInt32 						m_imageXSizeChanged;
	// Y size of the raster, recording the last time setting
	FdoInt32 						m_imageYSizeChanged;

	// raster data
	FdoByte*						m_buffer;
	FdoInt32						m_length;

	// the palette data, only available when the data model is palette
	RgbColor*						m_palette;
	FdoInt32						m_numberOfPaletteEntries;

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoWmsImage();
	FdoWmsImage(std::vector<GDALRasterBand*>& rasterBands);
	void Dispose() { delete this; }
	virtual ~FdoWmsImage(void);

public:
	static FdoWmsImage* Create(std::vector<GDALRasterBand*>& rasterBands)
	{
		return new FdoWmsImage(rasterBands);
	}

	// information for sub bands
	void AddBand(GDALRasterBand* band); 
	FdoInt32 GetBandSize();

	// raster data properties : data model, image size, null pixel value.
	FdoRasterDataModel* GetDataModel();
	void SetDataModel(FdoRasterDataModel* dataModel);

	FdoInt32 GetImageXSize();
	FdoInt32 GetImageYSize();

	void SetImageXSize(FdoInt32 size);
	void SetImageYSize(FdoInt32 size);

	FdoInt32 GetBytesPerRow();

	FdoDataValue* GetNullPixelValue();

	FdoInt32 GetNumberOfPaletteEntries();
	RgbColor* GetPalette();

	FdoByte* GetBuffer();
	FdoInt64 GetLength();

// Internally used funtions
private:
	void _getImageSize();
	void _getDataModel();
	void _getNullPixelValue();	
	void _getBuffer();
	void _destroyBuffer();
};

typedef FdoPtr<FdoWmsImage> FdoWmsImageP;

class FdoWmsImageCollection : public FdoCollection<FdoWmsImage, FdoCommandException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoWmsImageCollection();
    virtual ~FdoWmsImageCollection(void);
	void Dispose();
public:
	static FdoWmsImageCollection* Create();

};

typedef FdoPtr<FdoWmsImageCollection> FdoWmsImagesP;


#endif

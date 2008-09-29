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
#include <FdoWmsBandRasterGdal.h>
#include <FdoWmsRasterCapabilities.h>
#include <FdoWmsRect.h>
#include <FdoWmsEnvelope.h>
#include <FdoWmsStreamReaderGdalByRow.h>
#include <FdoWmsImage.h>
#include <FdoWmsRasterPropertyDictionaryGdal.h>
#include <Geometry/EnvelopeImpl.h>

FdoWmsBandRasterGdal::FdoWmsBandRasterGdal(FdoPtr<FdoWmsImage>& image, FdoWmsRect& bounds)
			: m_bNull(false), m_image(image), m_palette(NULL)
{
    FdoPtr<FdoFgfGeometryFactory> geomfactory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoEnvelopeImpl> envelope = FdoEnvelopeImpl::Create(bounds.m_minX, bounds.m_minY, bounds.m_maxX, bounds.m_maxY);
    FdoPtr<FdoIGeometry> iGeometery = geomfactory->CreateGeometry(envelope);
	m_bounds = geomfactory->GetFgf(iGeometery);
}

FdoWmsBandRasterGdal::~FdoWmsBandRasterGdal()
{
}

void FdoWmsBandRasterGdal::Dispose()
{
	delete this;
}

bool FdoWmsBandRasterGdal::IsNull ()
{
	return m_bNull;
}

void FdoWmsBandRasterGdal::SetNull ()
{
	m_bNull = true;
	m_bounds = NULL;
}

FdoDataValue* FdoWmsBandRasterGdal::GetNullPixelValue ()
{
	_validate();
	return m_image->GetNullPixelValue();
}

FdoByteArray* FdoWmsBandRasterGdal::GetBounds ()
{
	_validate();
	return FDO_SAFE_ADDREF(m_bounds.p);
}

void FdoWmsBandRasterGdal::SetBounds (FdoByteArray* bounds)
{
	if (bounds == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoWmsBandRasterGdal::SetBounds",
                                                               L"bounds"));

	_validate();
	m_bounds = FDO_SAFE_ADDREF(bounds);
}

FdoRasterDataModel* FdoWmsBandRasterGdal::GetDataModel ()
{
	_validate();
	return m_image->GetDataModel();
}

void FdoWmsBandRasterGdal::SetDataModel (FdoRasterDataModel* dataModel)
{
	if (dataModel == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoWmsBandRasterGdal::SetDataModel",
                                                               L"dataModel"));

	_validate();
	FdoPtr<FdoWmsRasterCapabilities> rasterCap = new FdoWmsRasterCapabilities();
	if (!rasterCap->SupportsDataModel(dataModel))
		throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11002_DATAMODEL_NOT_SUPPORTED, "Raster data model not supported."));

	m_image->SetDataModel(dataModel);
}

FdoInt32 FdoWmsBandRasterGdal::GetImageXSize ()
{
	_validate();
	return m_image->GetImageXSize();
}

void FdoWmsBandRasterGdal::SetImageXSize (FdoInt32 size)
{
    if (size <= 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoWmsBandRasterGdal::SetImageXSize",
                                                               L"size"));

	_validate();
	m_image->SetImageXSize(size);
}

FdoInt32 FdoWmsBandRasterGdal::GetImageYSize ()
{
	_validate();
	return m_image->GetImageYSize();
}

void FdoWmsBandRasterGdal::SetImageYSize (FdoInt32 size)
{
    if (size <= 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoWmsBandRasterGdal::SetImageYSize",
                                                               L"size"));

	_validate();
	m_image->SetImageYSize(size);
}

/// <returns>The dictionary of auxilliary raster properties.</returns> 
FdoIRasterPropertyDictionary* FdoWmsBandRasterGdal::GetAuxiliaryProperties ()
{
	return new FdoWmsRasterPropertyDictionayGdal(this);
}

void FdoWmsBandRasterGdal::SetStreamReader (FdoIStreamReader* reader)
{
	_validate();
}

FdoIStreamReader* FdoWmsBandRasterGdal::GetStreamReader ()
{
	_validate();
	return new FdoWmsStreamReaderGdalByRow(m_image);
}

RgbColor*	FdoWmsBandRasterGdal::GetPalette()
{
	_validate();
	return m_image->GetPalette();
}

FdoInt32 FdoWmsBandRasterGdal::GetNumberOfPaletteEntries()
{
	_validate();
	return m_image->GetNumberOfPaletteEntries();
}

// --------------------------------------------------------------------------
// Internally-used helper functions
// --------------------------------------------------------------------------
// Validate the raster is not null before any data retrieval operations
void FdoWmsBandRasterGdal::_validate()
{
	if (m_bNull || m_image == NULL)
		throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11001_RASTER_NULL, "Operation failed because the raster is null."));
}


FdoWmsBandRasterGdalCollection* FdoWmsBandRasterGdalCollection::Create()
{
	return new FdoWmsBandRasterGdalCollection();
}

void FdoWmsBandRasterGdalCollection::Dispose()
{
	delete this;
}

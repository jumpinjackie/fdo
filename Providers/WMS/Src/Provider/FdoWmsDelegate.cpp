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

#include <stdafx.h>
#include <OWS/FdoOwsGetCapabilities.h>
#include <OWS/FdoOwsResponse.h>
#include "FdoWmsDelegate.h"
#include "FdoWmsServiceMetadata.h"
#include "FdoWmsGetMap.h"
#include "FdoWmsXmlGlobals.h"

FdoWmsDelegate::FdoWmsDelegate() :
     FdoOwsDelegate(NULL, NULL, NULL)
{
}

FdoWmsDelegate::FdoWmsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd) :
     FdoOwsDelegate(defaultUrl, userName, passwd)
{
}

FdoWmsDelegate::~FdoWmsDelegate()
{
}

void FdoWmsDelegate::Dispose()
{ 
    delete this; 
}

FdoWmsDelegate* FdoWmsDelegate::Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd)
{
    return new FdoWmsDelegate(defaultUrl, userName, passwd);
}

FdoWmsServiceMetadata* FdoWmsDelegate::GetServiceMetadata()
{
    FdoPtr<FdoOwsGetCapabilities> request = FdoOwsGetCapabilities::Create(FdoWmsXmlGlobals::WMSServiceName);
    request->SetVersion(FdoWmsXmlGlobals::WmsVersion);
    FdoPtr<FdoOwsResponse> response = Invoke(request);
    FdoPtr<FdoIoStream> stream = response->GetStream();
    FdoWmsServiceMetadataP rv = FdoWmsServiceMetadata::Create();
    rv->ReadXml(stream);

    return FDO_SAFE_ADDREF(rv.p);
}

FdoIoStream* FdoWmsDelegate::GetMap(FdoStringCollection* layerNames,
									FdoStringCollection* styleNames,
									FdoWmsBoundingBox* bbox, 
									FdoString* imgFormat,
									FdoSize height,
									FdoSize width,
									FdoBoolean bTransparent,
									FdoString* backgroundColor,
									FdoString* timeDimension,
									FdoString* elevation,
									FdoString* version)
{
	VALIDATE_ARGUMENT (layerNames);
	VALIDATE_ARGUMENT (styleNames);
	VALIDATE_ARGUMENT (bbox);
	VALIDATE_ARGUMENT (imgFormat);
	VALIDATE_ARGUMENT (backgroundColor);
	VALIDATE_ARGUMENT (timeDimension);
	VALIDATE_ARGUMENT (elevation);
	
	FdoStringP crs = bbox->GetCRS ();
	FdoDouble minX = bbox->GetMinX ();
	FdoDouble minY = bbox->GetMinY ();
	FdoDouble maxX = bbox->GetMaxX ();
	FdoDouble maxY = bbox->GetMaxY ();

	FdoPtr<FdoWmsGetMap> request = FdoWmsGetMap::Create (layerNames, styleNames, crs, imgFormat, height, width, minX, minY, maxX, maxY, version, bTransparent, backgroundColor, timeDimension, elevation);
    FdoPtr<FdoOwsResponse> response = Invoke (request);
	FdoPtr<FdoIoStream> stream = response->GetStream ();

	return FDO_SAFE_ADDREF (stream.p);
}

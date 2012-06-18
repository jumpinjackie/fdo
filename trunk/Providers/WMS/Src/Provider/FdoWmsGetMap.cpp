/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "FdoWmsGetMap.h"
#include "FdoWmsXmlGlobals.h"
#include <FdoCommonStringUtil.h>
#include <OWS/FdoOwsGlobals.h>
#include "FdoWmsReverseEpsgCodes.h"
#include "FdoWmsGlobals.h"

FdoWmsGetMap::FdoWmsGetMap () :   
		FdoOwsRequest(FdoWmsXmlGlobals::WMSServiceName, FdoWmsXmlGlobals::WmsGetMapRequest), 
		mMinX(0.0), 
		mMinY(0.0), 
		mMaxX(0.0), 
		mMaxY(0.0),
		mbTransparent(false),
		mBackgroundColor(L"0xFFFFFF")
{
	FdoOwsRequest::SetVersion (FdoWmsXmlGlobals::WmsVersion);
}

FdoWmsGetMap::FdoWmsGetMap (FdoStringCollection* layerNames, 
							FdoStringCollection* styleNames, 
							FdoString* srsName, 
							FdoString* imgFormat, 
							FdoSize height, 
							FdoSize width, 
							FdoDouble minx, 
							FdoDouble miny, 
							FdoDouble maxx, 
							FdoDouble maxy,
							FdoString* version,
							FdoString* exceptionFormat,
							FdoBoolean bTransparent, 
							FdoString* backgroundColor, 
							FdoString* timeDimension, 
							FdoString* elevation) 
	:   FdoOwsRequest(FdoWmsXmlGlobals::WMSServiceName, FdoWmsXmlGlobals::WmsGetMapRequest),         
        mSrsName(srsName), 
        mFormat(imgFormat),
		mHeight(height),
		mWidth(width),
        mMinX(minx), 
        mMinY(miny), 
        mMaxX(maxx), 
        mMaxY(maxy),
		mExceptionFormat(exceptionFormat),
		mbTransparent(bTransparent),
		mBackgroundColor(backgroundColor ? backgroundColor : L""),
		mTimeDimension(timeDimension ? timeDimension : L""),
		mElevation(elevation ? elevation : L"")		
{
	FdoOwsRequest::SetVersion (version ? version : FdoWmsXmlGlobals::WmsVersion);
	mLayerNames = FDO_SAFE_ADDREF (layerNames);
	mStyleNames = FDO_SAFE_ADDREF (styleNames);
}

FdoWmsGetMap* FdoWmsGetMap::Create (FdoStringCollection* layerNames, 
									FdoStringCollection* styleNames, 
									FdoString* srsName, 
									FdoString* imgFormat,
									FdoSize height,
									FdoSize width,
									FdoDouble minx, 
									FdoDouble miny, 
									FdoDouble maxx, 
									FdoDouble maxy,
									FdoString* version,
									FdoString* exceptionFormat,
									FdoBoolean bTransparent, 
									FdoString* backgroundColor, 
									FdoString* timeDimension, 
									FdoString* elevation)
{
	if (layerNames == NULL || layerNames->GetCount () == 0)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoWmsGetMap",
                                                               L"layerNames"));

	return new FdoWmsGetMap (layerNames, styleNames, srsName, imgFormat, height, width, minx, miny, maxx, maxy, version, exceptionFormat,bTransparent, backgroundColor, timeDimension, elevation);
}

FdoWmsGetMap::~FdoWmsGetMap ()
{
}

FdoStringP FdoWmsGetMap::EncodeKVP()
{
	// For common request, version and service
    FdoStringP ret = FdoOwsRequest::EncodeKVP();

    ret += FdoOwsGlobals::And;
    ret += _encodeGetMapURL(
        mLayerNames,
        mStyleNames,
        mSrsName,
        mFormat,
        mMaxX,
        mMinX,
        mMaxY,
        mMinY,
        mHeight,
        mWidth,
        mbTransparent,
        mBackgroundColor,
        mTimeDimension,
        mElevation,
        mExceptionFormat,
        m_version
        );

    // Translate to a valid HTTP request
    // NOTE: Currently, this is mainly for handling the blank space in "FORMAT"
    ret = ret.Replace(L" ", L"%20"); 

	return ret;
}

FdoStringP FdoWmsGetMap::EncodeXml()
{
	// TODO: shall we support this?
	return L"";
}


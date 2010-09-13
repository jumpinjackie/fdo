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
    ret += FdoWmsXmlGlobals::EXCEPTIONS;
    ret += FdoOwsGlobals::Equal;
	if (mExceptionFormat.GetLength())
		ret += mExceptionFormat;

	// Add "LAYERS" parameters in the request	
	ret += FdoOwsGlobals::And;
	ret += FdoWmsXmlGlobals::WmsRequestLayers;
	ret += FdoOwsGlobals::Equal;
	
	ret += UrlEscape(mLayerNames->GetString (0));
	for (FdoInt32 i=1; i<mLayerNames->GetCount (); i++)
	{
		ret += FdoWmsXmlGlobals::WmsRequestComma;
		ret += UrlEscape(mLayerNames->GetString (i));			
	}
		
	// Add "STYLES" parameters in the request
	if (mStyleNames && mStyleNames->GetCount () > 0)
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestStyles;
		ret += FdoOwsGlobals::Equal;
		ret += UrlEscape(mStyleNames->GetString (0));
		for (FdoInt32 i=1; i<mStyleNames->GetCount (); i++)
		{
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += UrlEscape(mStyleNames->GetString (i));
		}
	}
	else // Use "default" styles to please non-strict servers
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestStyles;
		ret += FdoOwsGlobals::Equal;
		ret += FdoWmsXmlGlobals::WmsRequestDefaultStyle;
		for (FdoInt32 i=1; i<mLayerNames->GetCount (); i++)
		{
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoWmsXmlGlobals::WmsRequestDefaultStyle;
		}
	}

	// Some server require "SRS" parameter instead of "CRS" in the request. However
	// others require "CRS" provided. Here is a workaround to use both of them at
	// the same time. And hope the servers will feel happy.
	ret += FdoOwsGlobals::And;
	ret += FdoWmsXmlGlobals::WmsRequestCRS;
	ret += FdoOwsGlobals::Equal;
	ret += mSrsName; // Don't escape, even though there is a ":", ErMapper services don't like this

	ret += FdoOwsGlobals::And;
	ret += FdoWmsXmlGlobals::WmsRequestSRS;
	ret += FdoOwsGlobals::Equal;
	ret += mSrsName; // Don't escape, even though there is a ":", ErMapper services don't like this

	// Add "FORMAT" in the request
	ret += FdoOwsGlobals::And;
	ret += FdoWmsXmlGlobals::WmsRequestFormat;
	ret += FdoOwsGlobals::Equal;
	ret += mFormat; // Don't escape, even though there is a "/", ErMapper services don't like this

	// Add the "BBOX" in the request
	double areaSize = (mMaxX - mMinX) * (mMaxY - mMinY);
	if (areaSize > 0)
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestBBOX;
		ret += FdoOwsGlobals::Equal;

		// handle the reverse at the last minute before sending request
		FdoBoolean reverse;

		if (wcscmp(FdoWmsGlobals::WmsVersion100,m_version) == 0 ||
			wcscmp(FdoWmsGlobals::WmsVersion110,m_version) == 0 ||
			wcscmp(FdoWmsGlobals::WmsVersion111,m_version) == 0)
			reverse = false; 
		else
		{
			// check reverse only when current version is 1.3.0 or later and cs is EPSG format
			if (mSrsName.Contains(FdoWmsGlobals::ESPGPrefix))
				reverse = _reverseCheck(mSrsName);
		}

		if (reverse)
		{
			ret += FdoStringP::Format(L"%lf", mMinY);
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoStringP::Format(L"%lf", mMinX);
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoStringP::Format(L"%lf", mMaxY);
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoStringP::Format(L"%lf", mMaxX);
		}
		else
		{
			ret += FdoStringP::Format(L"%lf", mMinX);
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoStringP::Format(L"%lf", mMinY);
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoStringP::Format(L"%lf", mMaxX);
			ret += FdoWmsXmlGlobals::WmsRequestComma;
			ret += FdoStringP::Format(L"%lf", mMaxY);
		}
	}

	// Add the "HEIGHT" and "WIDTH" parameters
	if (mHeight > 0 && mWidth > 0)
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestHeight;
		ret += FdoOwsGlobals::Equal;
		ret += FdoStringP::Format(L"%d", mHeight);
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestWidth;
		ret += FdoOwsGlobals::Equal;
		ret += FdoStringP::Format(L"%d", mWidth);
	}

	// For "TRANSPARENT" parameter
	ret += FdoOwsGlobals::And;
	ret += FdoWmsXmlGlobals::WmsRequestTransparent;
	ret += FdoOwsGlobals::Equal;
	if (mbTransparent)
	{
		ret += FdoWmsXmlGlobals::WmsRequestTransparentTrue;
	}
	else
	{
		ret += FdoWmsXmlGlobals::WmsRequestTransparentFalse;
	}

	// For "BGCOLOR" parameter
	if (mBackgroundColor.GetLength ())
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestBackgroundColor;
		ret += FdoOwsGlobals::Equal;
		ret += mBackgroundColor;
	}

	// For "TIME" paramater in the request
	if (mTimeDimension.GetLength ())
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestTimeDimension;
		ret += FdoOwsGlobals::Equal;
		ret += mTimeDimension;
	}

	// For "ELEVATION" parameter
	if (mElevation.GetLength ())
	{
		ret += FdoOwsGlobals::And;
		ret += FdoWmsXmlGlobals::WmsRequestElevation;
		ret += FdoOwsGlobals::Equal;
		ret += mElevation;
	}
    
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


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
#include "FdoWmsLayer.h"
#include "FdoWmsLayerCollection.h"
#include "FdoWmsBoundingBox.h"
#include "FdoWmsBoundingBoxCollection.h"
#include "FdoWmsXmlGlobals.h"
#include <OWS/FdoOwsGlobals.h>

FdoWmsLayer::FdoWmsLayer(void) :
    mQueryable(false),
    mOpaque(false),
    mNoSubsets(false),
    mFixedWidth(-1),
    mFixedHeight(-1),
	mParent(NULL),
    mbSkipProcessingElement(false),
    mProcessed(false)
{
    mKeywordList = FdoStringCollection::Create();
    mCoordinateReferenceSystems = FdoStringCollection::Create();
    mChildLayers = FdoWmsLayerCollection::Create();
    mStyles = FdoWmsStyleCollection::Create();
    mDimensions = FdoWmsDimensionCollection::Create();
    mBoundingBoxes = FdoWmsBoundingBoxCollection::Create();
}

FdoWmsLayer::~FdoWmsLayer(void)
{
}

void FdoWmsLayer::Dispose()
{
    delete this;
}

FdoWmsLayer* FdoWmsLayer::Create(void)
{
    return new FdoWmsLayer();
}

void FdoWmsLayer::InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs)
{
    try {
        VALIDATE_ARGUMENT(attrs);
        VALIDATE_ARGUMENT(context);

        FdoXmlAttributeP att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesLayerQueryable);
        if (att != NULL) {
            FdoBoolean queryable = false;
            FdoStringP value = att->GetValue();
            if (wcscmp(value, FdoWmsXmlGlobals::WmsCapabilitiesTrueString) == 0) {
                queryable = true;
            }
            SetQueryable(queryable);
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesLayerOpaque);
        if (att != NULL) {
            FdoBoolean opaque = false;
            FdoStringP value = att->GetValue();
            if (wcscmp(value, FdoWmsXmlGlobals::WmsCapabilitiesTrueString) == 0) {
                opaque = true;
            }
            SetOpaque(opaque);
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesLayerNoSubsets);
        if (att != NULL) {
            FdoBoolean noSubsets = false;
            FdoStringP value = att->GetValue();
            if (wcscmp(value, FdoWmsXmlGlobals::WmsCapabilitiesTrueString) == 0) {
                noSubsets = true;
            }
            SetNoSubsets(noSubsets);
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesLayerFixedWidth);
        if (att != NULL) {
            FdoStringP value = att->GetValue();
            FdoInt32 fixedWidth = atol(value);
            SetFixedWidth(fixedWidth);
        }

        att = attrs->FindItem(FdoWmsXmlGlobals::WmsCapabilitiesLayerFixedHeight);
        if (att != NULL) {
            FdoStringP value = att->GetValue();
            FdoInt32 fixedHeight = atol(value);
            SetFixedHeight(fixedHeight);
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }
}

FdoXmlSaxHandler* FdoWmsLayer::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
	FdoXmlSaxHandler* pRet = NULL;

	try
	{
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
			if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerAttribution) == 0)
            {
	            mbSkipProcessingElement = true;
                return pRet;
            }

			if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerName) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerTitle) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerAbstract) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerKeyword) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesSRS) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesCRS) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerMinScale) == 0 ||
			    FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerMaxScale) == 0)
            {
                mXmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = mXmlContentHandler;
            }
			else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesBoundingBox) == 0) {
                FdoWmsBoundingBoxP pBBox = FdoWmsBoundingBox::Create();
                pBBox->InitFromXml(context, atts);

                FdoWmsBoundingBoxCollectionP pBoundingBoxes = GetBoundingBoxes();
                pBoundingBoxes->Add(pBBox);

                pRet = pBBox;
            }
			else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayer) == 0) {
                FdoWmsLayerP pLayer = FdoWmsLayer::Create();
                pLayer->InitFromXml(context, atts);

                FdoWmsLayerCollectionP pLayers = GetLayers();
                pLayers->Add(pLayer);
                pLayer->SetParent(this);

                pRet = pLayer;
			}
			else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesStyle) == 0) {
                FdoWmsStyleP pStyle = FdoWmsStyle::Create();
                pStyle->InitFromXml(context, atts);

                FdoWmsStyleCollectionP pStyles = GetStyles();
                pStyles->Add(pStyle);

                pRet = pStyle;
			}
			else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesDimension) == 0) {
                FdoWmsDimensionP pDimension = FdoWmsDimension::Create();
                pDimension->InitFromXml(context, atts);

                FdoWmsDimensionCollectionP pDimensions = GetDimensions();
                pDimensions->Add(pDimension);

                pRet = pDimension;
			}
			else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesExGeoBoundingBox) == 0 ||
                     FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::CapabilitiesLatLonBoundingBox) == 0) 
            {
                if (mGeographicBoundingBox == NULL)
					mGeographicBoundingBox = FdoOwsGeographicBoundingBox::Create();
                mGeographicBoundingBox->InitFromXml(context, atts);

                pRet = mGeographicBoundingBox;
			}
		}
	}
	catch (FdoException* e)
	{
		context->AddError(e);
		e->Release();
	}

	return pRet;
}

FdoBoolean FdoWmsLayer::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    try 
    {
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        if (!mbSkipProcessingElement)
        {
            if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerName) == 0) {
                SetName(mXmlContentHandler->GetString());
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerTitle) == 0) {
                SetTitle(mXmlContentHandler->GetString());
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerAbstract) == 0) {
                SetAbstract(mXmlContentHandler->GetString());
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerKeyword) == 0) {
                FdoStringsP keywords = GetKeyordList();
                keywords->Add(mXmlContentHandler->GetString());
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerMinScale) == 0) {
                SetMinScaleDemoninator(FdoCommonOSUtil::wtof(mXmlContentHandler->GetString()));
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerMaxScale) == 0) {
                SetMaxScaleDemoninator(FdoCommonOSUtil::wtof(mXmlContentHandler->GetString()));
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesSRS) == 0 ||
                     FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesCRS) == 0) 
            {
                FdoStringsP layerCoordSystems = GetCoordinateReferenceSystems();
                FdoStringsP coordSystems = FdoStringCollection::Create(mXmlContentHandler->GetString(), FdoWmsXmlGlobals::WmsCapabilitiesSRSDelimiter);
                layerCoordSystems += coordSystems;
            }
        }

        if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayerAttribution) == 0) {
            mbSkipProcessingElement = false;
        }

        FDO_SAFE_RELEASE(mXmlContentHandler.p);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return BaseType::XmlEndElement(context, uri, name, qname);
}

FdoString* FdoWmsLayer::GetName() const
{
    return mName;
}

void FdoWmsLayer::SetName(FdoString* name)
{
    mName = name;
}

FdoBoolean FdoWmsLayer::GetQueryable() const
{
    return mQueryable;
}

void FdoWmsLayer::SetQueryable(FdoBoolean queryable)
{
    mQueryable = queryable;
}

FdoBoolean FdoWmsLayer::GetOpaque() const
{
    return mOpaque;
}

void FdoWmsLayer::SetOpaque(FdoBoolean opaque)
{
    mOpaque = opaque;
}

FdoBoolean FdoWmsLayer::GetNoSubsets() const
{
    return mNoSubsets;
}

void FdoWmsLayer::SetNoSubsets(FdoBoolean noSubsets)
{
    mNoSubsets = noSubsets;
}

FdoString* FdoWmsLayer::GetTitle() const
{
    return mTitle;
}

void FdoWmsLayer::SetTitle(FdoString* title)
{
    mTitle = title;
}

FdoString* FdoWmsLayer::GetAbstract() const
{
    return mAbstract;
}

void FdoWmsLayer::SetAbstract(FdoString* abstract)
{
    mAbstract = abstract;
}

FdoInt32 FdoWmsLayer::GetFixedWidth() const
{
    return mFixedWidth;
}

void FdoWmsLayer::SetFixedWidth(FdoInt32 width)
{
    mFixedWidth = width;
}

FdoInt32 FdoWmsLayer::GetFixedHeight() const
{
    return mFixedHeight;
}

void FdoWmsLayer::SetFixedHeight(FdoInt32 height)
{
    mFixedHeight = height;
}

FdoStringCollection* FdoWmsLayer::GetKeyordList() const
{
    return FDO_SAFE_ADDREF(mKeywordList.p);
}

FdoStringCollection* FdoWmsLayer::GetCoordinateReferenceSystems() const
{
    return FDO_SAFE_ADDREF(mCoordinateReferenceSystems.p);
}

bool FdoWmsLayer::FindCoordinateReferenceSystem (FdoString* coordSystem)
{
	bool bFound = false;
	FdoStringsP crsNames = GetCoordinateReferenceSystems ();
	if (crsNames != NULL)
	{
		bFound = (crsNames->IndexOf (coordSystem) != -1);
	}

	if (!bFound)
	{
		FdoWmsLayerP parent = this->GetParent ();
		if (parent != NULL)
		{
			bFound = parent->FindCoordinateReferenceSystem (coordSystem);
		}
	}

	return bFound;
}

FdoWmsLayerCollection* FdoWmsLayer::GetLayers() const
{
    return FDO_SAFE_ADDREF(mChildLayers.p);
}

FdoWmsStyleCollection* FdoWmsLayer::GetStyles() const
{
    return FDO_SAFE_ADDREF(mStyles.p);
}

FdoWmsDimensionCollection* FdoWmsLayer::GetDimensions() const
{
    return FDO_SAFE_ADDREF(mDimensions.p);
}

FdoWmsBoundingBoxCollection* FdoWmsLayer::GetBoundingBoxes() const
{
    return FDO_SAFE_ADDREF(mBoundingBoxes.p);
}

FdoDouble FdoWmsLayer::GetMinScaleDemoninator() const
{
    return mMinScaleDenominator;
}

void FdoWmsLayer::SetMinScaleDemoninator(FdoDouble scale)
{
    mMinScaleDenominator = scale;
}

FdoDouble FdoWmsLayer::GetMaxScaleDemoninator() const
{
    return mMaxScaleDenominator;
}

void FdoWmsLayer::SetMaxScaleDemoninator(FdoDouble scale)
{
    mMaxScaleDenominator = scale;
}

FdoOwsGeographicBoundingBox* FdoWmsLayer::GetGeographicBoundingBox() const
{
	return FDO_SAFE_ADDREF (mGeographicBoundingBox.p);
}

void FdoWmsLayer::SetGeographicBoundingBox(FdoOwsGeographicBoundingBox* value)
{
	mGeographicBoundingBox = value;
}

FdoWmsLayer* FdoWmsLayer::GetParent ()
{
	return FDO_SAFE_ADDREF(mParent);
}

void FdoWmsLayer::SetParent (FdoWmsLayer* parent)
{
	// Soft reference to avoid circular reference. DO NOT ADDREF
    mParent = parent;
}

FdoBoolean FdoWmsLayer::GetLayerProcessed() const
{
    return mProcessed;
}
void FdoWmsLayer::SetLayerProcessed(FdoBoolean state)
{
    mProcessed = state;
}

